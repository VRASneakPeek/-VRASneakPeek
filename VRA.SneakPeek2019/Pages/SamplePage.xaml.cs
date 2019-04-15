using Microsoft.Azure.Devices;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Runtime.CompilerServices;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Input;
using VRA.SneakPeek2019.CustomVision;
using VRA.SneakPeek2019.Factories;
using VRA.SneakPeek2019.Handlers;
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

namespace VRA.SneakPeek2019.Pages
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class SamplePage : Page, INotifyPropertyChanged
    {
        /// <inheritdoc />
        public event PropertyChangedEventHandler PropertyChanged;

        private readonly ServiceClient _serviceClient;
        private readonly MediaCapture _mediaCapture;
        private readonly DisplayRequest _displayRequest;
        private readonly SemaphoreSlim _frameProcessingSemaphore;

        private VideoEncodingProperties _videoProperties;
        private ObjectDetection _objectDetection;

        private bool _isVideoCapturing;

        private ICommand _onLeft;
        private ICommand _onRight;
        private ICommand _onStartVideoCapture;

        private string DeviceId = "<<FILL_IN_NAME_HERE>>";

        /// <summary>
        /// Gets a value indicating whether this instance is capturing video.
        /// </summary>
        /// <value>
        ///   <c>true</c> if this instance is capturing video; otherwise, <c>false</c>.
        /// </value>
        public bool IsVideoCapturing
        {
            get => _isVideoCapturing;
            private set => SetField(ref _isVideoCapturing, value);
        }

        /// <summary>
        /// Gets the command to go left.
        /// </summary>
        /// <value>
        /// The on left-command.
        /// </value>
        public ICommand OnLeft
        {
            get => _onLeft ?? (_onLeft = new CommandHandler(async () =>
            {
                try
                {
                    if (DeviceId != "<<FILL_IN_NAME_HERE>>")
                        await _serviceClient.InvokeDeviceMethodAsync(DeviceId, new CloudToDeviceMethod("burstLeft"));
                }
                catch (Exception)
                {
                   // ignored
                }
              
            }));
        }

        /// <summary>
        /// Gets the command to go right.
        /// </summary>
        /// <value>
        /// The on right-command.
        /// </value>
        public ICommand OnRight
        {
            get => _onRight ?? (_onRight = new CommandHandler(async () =>
            {
                try
                {
                    if (DeviceId != "<<FILL_IN_NAME_HERE>>")
                        await _serviceClient.InvokeDeviceMethodAsync(DeviceId, new CloudToDeviceMethod("burstRight"));
                }
                catch (Exception)
                {
                    // ignored
                }
            }));
        }

        /// <summary>
        /// Gets the command to start capturing video.
        /// </summary>
        /// <value>
        /// The on start video capture-command.
        /// </value>
        public ICommand OnStartVideoCapture
        {
            get => _onStartVideoCapture ?? (_onStartVideoCapture = new CommandHandler(async () =>
            {
                if (IsVideoCapturing)
                    return;
                IsVideoCapturing = true;

                TextBlockResults.Text = "";
                TextBlockResults.Visibility = Visibility.Visible;

                TextBlockInvokedMethod.Text = "";
                TextBlockInvokedMethod.Visibility = Visibility.Visible;

                await InitModel();
                await StartVideoPreviewAsync();
            }));
        }
        
        public SamplePage()
        {
            InitializeComponent();
            DataContext = this;

            _serviceClient = ServiceClientFactory.CreateServiceClient();
            _mediaCapture = new MediaCapture();
            _displayRequest = new DisplayRequest();
            _frameProcessingSemaphore = new SemaphoreSlim(1);
        }
        
        private async Task InitModel()
        {
            _objectDetection = new ObjectDetection();
            var modelFile = await StorageFile.GetFileFromApplicationUriAsync(new Uri($"ms-appx:///Assets/model.onnx"));
            await _objectDetection.Init(modelFile);
        }

        private async Task StartVideoPreviewAsync()
        {
            await _mediaCapture.InitializeAsync();
            _displayRequest.RequestActive();

            previewControl.Source = _mediaCapture;
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
                    //if (DeviceId != "<<FILL_IN_NAME_HERE>>")
                    //    await _serviceClient.InvokeDeviceMethodAsync(DeviceId, new CloudToDeviceMethod(method));

                    TextBlockInvokedMethod.Text = method + Environment.NewLine + "invoked";
                    GestureState.ExecutedMethod = method;

                    GestureState.OpenHandOccurences = 0;
                    GestureState.CloseHandOccurences = 0;

                    Debug.WriteLine(method + " invoked");
                }
                catch (Exception ex)
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

        /// <summary>
        /// Sets the field.
        /// </summary>
        /// <typeparam name="T">Type of the field.</typeparam>
        /// <param name="field">The field.</param>
        /// <param name="value">The value.</param>
        /// <param name="propertyName">Name of the property.</param>
        /// <returns><c>TRUE</c> if the field received a new value; otherwise, <c>FALSE</c>.</returns>
        private bool SetField<T>(ref T field, T value, [CallerMemberName]string propertyName = "")
        {
            if (EqualityComparer<T>.Default.Equals(field, value))
                return false;

            field = value;
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));

            return true;
        }
    }
}
