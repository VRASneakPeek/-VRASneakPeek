using System.Windows;
using System.Windows.Input;

namespace ScoreBoard
{
    public partial class MainWindow
    {
        private ICommand _dustEaterOneInvoked;
        private ICommand _dustEaterTwoInvoked;
        private ICommand _dustEaterThreeInvoked;
        private ICommand _darkKnightOneInvoked;
        private ICommand _darkKnightTwoInvoked;
        private ICommand _darkKnightThreeInvoked;

        public MainWindow()
        {
            InitializeComponent();

            DarkKnightWins.Visibility = Visibility.Collapsed;
            DustEaterWins.Visibility = Visibility.Collapsed;

            DataContext = this;
        }

        public ICommand DustEaterOneInvoked => _dustEaterOneInvoked ?? (_dustEaterOneInvoked = new CommandHandler(() =>
        {
            DustEaterOne.IsEnabled = false;
            DustEaterOne.Content = "Passed!";
        }));

        public ICommand DustEaterTwoInvoked => _dustEaterTwoInvoked ?? (_dustEaterTwoInvoked = new CommandHandler(() =>
        {
            DustEaterTwo.IsEnabled = false;
            DustEaterTwo.Content = "Passed!";
        }));

        public ICommand DustEaterThreeInvoked => _dustEaterThreeInvoked ?? (_dustEaterThreeInvoked = new CommandHandler(() =>
        {
            DustEaterThree.IsEnabled = false;
            DustEaterThree.Content = "Passed!";

            DarkKnightOne.Visibility = Visibility.Collapsed;
            DarkKnightTwo.Visibility = Visibility.Collapsed;
            DarkKnightThree.Visibility = Visibility.Collapsed;
            LogoDarkKNight.Visibility = Visibility.Collapsed;
            TextDarkKNight.Visibility = Visibility.Collapsed;

            DustEaterWins.Visibility = Visibility.Visible;

        }));

        public ICommand DarkKnightOneInvoked => _darkKnightOneInvoked ?? (_darkKnightOneInvoked = new CommandHandler(() =>
        {
            DarkKnightOne.IsEnabled = false;
            DarkKnightOne.Content = "Passed!";
        }));

        public ICommand DarkKnightTwoInvoked => _darkKnightTwoInvoked ?? (_darkKnightTwoInvoked = new CommandHandler(() =>
        {
            DarkKnightTwo.IsEnabled = false;
            DarkKnightTwo.Content = "Passed!";
        }));

        public ICommand DarkKnightThreeInvoked => _darkKnightThreeInvoked ?? (_darkKnightThreeInvoked = new CommandHandler(() =>
        {
            DarkKnightThree.IsEnabled = false;
            DarkKnightThree.Content = "Passed!";

            DustEaterOne.Visibility = Visibility.Collapsed;
            DustEaterTwo.Visibility = Visibility.Collapsed;
            DustEaterThree.Visibility = Visibility.Collapsed;
            LogoDustEater.Visibility = Visibility.Collapsed;
            TextDustEater.Visibility = Visibility.Collapsed;

            DarkKnightWins.Visibility = Visibility.Visible;

        }));
    }
}
