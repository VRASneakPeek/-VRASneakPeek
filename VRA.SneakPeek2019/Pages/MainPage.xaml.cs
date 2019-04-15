using Microsoft.Azure.Devices;
using System;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;
using VRA.SneakPeek2019.CustomVision;
using VRA.SneakPeek2019.States;
using Windows.Graphics.Imaging;
using Windows.Media;
using Windows.Media.Capture;
using Windows.Media.MediaProperties;
using Windows.Storage;
using Windows.System.Display;
using Windows.System.Threading;
using Windows.UI;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Media;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace VRA.SneakPeek2019.Pages
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        private readonly DisplayRequest _displayRequest = new DisplayRequest();
        private readonly MediaCapture _mediaCapture = new MediaCapture();
        private readonly SemaphoreSlim _frameProcessingSemaphore = new SemaphoreSlim(1);
        private VideoEncodingProperties _videoProperties;
        private ObjectDetection _objectDetection;
        private readonly ServiceClient _serviceClient;
        
        public MainPage()
        {
            InitializeComponent();
            TextBlockInvokedMethod.Text = "";
            TextBlockResults.Text = "";

            InitModel();
            StartVideoPreviewAsync();
            _serviceClient = ServiceClient.CreateFromConnectionString(
                "HostName=VRATelemetryIoTHubDev.azure-devices.net;SharedAccessKeyName=iothubowner;" +
                "SharedAccessKey=suxuL4aegXhGE+yB/hf1c8zqisz4XdIN0YlAnJA3Xbc=");
        }

        private async Task StartVideoPreviewAsync()
        {
            await _mediaCapture.InitializeAsync();
            _displayRequest.RequestActive();

            PreviewControl.Source = _mediaCapture;
            await _mediaCapture.StartPreviewAsync();

            var timerInterval = TimeSpan.FromMilliseconds(66); //15fps
            ThreadPoolTimer.CreatePeriodicTimer(ProcessCurrentVideoFrame, timerInterval);
            _videoProperties = _mediaCapture.VideoDeviceController.GetMediaStreamProperties(MediaStreamType.VideoPreview) as VideoEncodingProperties;
        }
        private async void ProcessCurrentVideoFrame(ThreadPoolTimer timer)
        {
            if (_mediaCapture.CameraStreamState != Windows.Media.Devices.CameraStreamState.Streaming ||
                !_frameProcessingSemaphore.Wait(0))
                return;

            try
            {
                using (var previewFrame = new VideoFrame(BitmapPixelFormat.Bgra8, (int)_videoProperties.Width, (int)_videoProperties.Height))
                {
                    await _mediaCapture.GetPreviewFrameAsync(previewFrame);
                    await Task.Run(async () =>
                    {
                        await LoadAndEvaluateModelAsync(previewFrame);
                    });
                }
            }
            catch
            {
                // ignored
            }
            finally
            {
                _frameProcessingSemaphore.Release();
            }
        }


        private async void InitModel()
        {
            _objectDetection = new ObjectDetection();
            var modelFile = await StorageFile.GetFileFromApplicationUriAsync(new Uri($"ms-appx:///Assets/model.onnx"));
            await _objectDetection.Init(modelFile);
        }

        private async Task LoadAndEvaluateModelAsync(VideoFrame videoFrame)
        {
            var result = await _objectDetection.PredictImageAsync(videoFrame);
            var message = $"{DateTime.Now.ToLongTimeString()}{Environment.NewLine}============================={Environment.NewLine}";

            PredictionModel chosenPredModel = null;
            foreach (var predictionModel in result)
            {
                var probability = (predictionModel.Probability * 100.0f);
                if (!(probability > 50)) continue;

                message += $" {predictionModel.TagName} {(predictionModel.Probability * 100.0f):#0.00}% {Environment.NewLine}";
                chosenPredModel = predictionModel;
            }

            if (chosenPredModel != null)
            {
                Debug.WriteLine(message);
                await Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () => TextBlockResults.Text = $"{message}");
                await Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () => ShowObjectDetectionBoxes(chosenPredModel));
                await Dispatcher.RunAsync(CoreDispatcherPriority.Normal, async () => await SteerRobotIfNeeded(chosenPredModel.TagName));
            }
        }

        private async Task SteerRobotIfNeeded(string tagName)
        {
            switch (tagName)
            {
                case "openhand":
                    GestureState.OpenHandOccurences += 1;
                    break;
                case "closehand":
                    GestureState.CloseHandOccurences += 1;
                    break;
            }

            if (GestureState.OpenHandOccurences == 3)
                await CallDeviceMethod("stopMoving");

            if (GestureState.CloseHandOccurences == 3)
                await CallDeviceMethod("moveForward");
        }

        public async Task CallDeviceMethod(string method)
        {
            if (method != GestureState.ExecutedMethod)
            {
                try
                {
                    await _serviceClient.InvokeDeviceMethodAsync("DarkKnight", new CloudToDeviceMethod(method));

                    TextBlockInvokedMethod.Text = method + Environment.NewLine + "invoked";
                    GestureState.ExecutedMethod = method;
                    GestureState.OpenHandOccurences = 0;
                    GestureState.CloseHandOccurences = 0;
                    Debug.WriteLine(method + " invoked");
                }
                catch (Exception)
                {
                    // ignored
                }
            }
        }

        private void ShowObjectDetectionBoxes(PredictionModel prediction)
        {
            objectDetectionVisualizationCanvas.Children.Clear();

            var canvasWidth = objectDetectionVisualizationCanvas.ActualWidth;
            var canvasHeight = objectDetectionVisualizationCanvas.ActualHeight;

            objectDetectionVisualizationCanvas.Children.Add(
                    new Border
                    {
                        BorderBrush = new SolidColorBrush(Colors.Lime),
                        BorderThickness = new Thickness(2),
                        Margin = new Thickness(prediction.BoundingBox.Left * canvasWidth,
                                               prediction.BoundingBox.Top * canvasHeight, 0, 0),
                        Width = prediction.BoundingBox.Width * canvasWidth,
                        Height = prediction.BoundingBox.Height * canvasHeight,
                    });

            objectDetectionVisualizationCanvas.Children.Add(
                    new Border
                    {
                        Height = 40,
                        FlowDirection = FlowDirection.LeftToRight,
                        HorizontalAlignment = HorizontalAlignment.Left,
                        Margin = new Thickness(prediction.BoundingBox.Left * canvasWidth,
                                               prediction.BoundingBox.Top * canvasHeight - 40, 0, 0),

                        Child = new Border
                        {
                            Background = new SolidColorBrush(Colors.Lime),
                            HorizontalAlignment = HorizontalAlignment.Left,
                            VerticalAlignment = VerticalAlignment.Bottom,
                            Child =
                                new TextBlock
                                {
                                    Foreground = new SolidColorBrush(Colors.Black),
                                    Text = $"{prediction.TagName} ({Math.Round(prediction.Probability * 100)}%)",
                                    FontSize = 16,
                                    Margin = new Thickness(6, 0, 6, 0)
                                }
                        }
                    });
        }
    }
}
