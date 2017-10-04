using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Turnomatic_Loader
{
    /// Interaction logic for config.xaml
    public partial class ConfigDialogBox : Window
    {

        private UsbConnection usb;

        public ConfigDialogBox()
        {
            InitializeComponent();
        }

        public UsbConnection Usb {
            set {
                usb = value;
            }
        }

        public void btn_readPot(object sender, RoutedEventArgs e)
        {
            if (usb.GetPotValue())
            {
                potValuesBox.Items.Insert(0, usb.PotValue.ToString());
            } else
            {
                // Generate error message
                MessageBox.Show("Error reading pot", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        public void btn_setPot(object sender, RoutedEventArgs e)
        {
            try
            {
                int val = int.Parse(posBox.Text);
                if (usb.SetPotValue(val))
                {
                    potValuesBox.Items.Insert(0, String.Format("({0})", val));
                } else
                {
                    MessageBox.Show("Error setting pot value", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                }
            } catch (FormatException)
            {
                MessageBox.Show("Error setting pot value", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        public void btn_getConfig(object sender, RoutedEventArgs e)
        {
            if (usb.GetConfigValues())
            {
                page1pos.Text = usb.Page1Pos.ToString();
                pagePosDiff.Text = usb.PagePosDiff.ToString();
                motorGain.Text = usb.MotorGain.ToString();
                micThresh.Text = usb.MicThresh.ToString();

                page1pos.FontStyle = FontStyles.Italic;
                pagePosDiff.FontStyle = FontStyles.Italic;
                motorGain.FontStyle = FontStyles.Italic;
                micThresh.FontStyle = FontStyles.Italic;
            }
            else
            {
                // Generate error message
                MessageBox.Show("Error reading config values", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        public void btn_setConfig(object sender, RoutedEventArgs e)
        {
            try
            {
                int _page1pos = int.Parse(page1pos.Text);
                int _pagePosDiff = int.Parse(pagePosDiff.Text);
                float _motorGain = float.Parse(motorGain.Text);
                float _micThresh = float.Parse(micThresh.Text);

                if (usb.SetConfigValues(_page1pos, _pagePosDiff, _motorGain, _micThresh))
                {
                    page1pos.FontStyle = FontStyles.Italic;
                    pagePosDiff.FontStyle = FontStyles.Italic;
                    motorGain.FontStyle = FontStyles.Italic;
                    micThresh.FontStyle = FontStyles.Italic;
                } else
                {
                    MessageBox.Show("Error setting config values", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                }

            }
            catch (FormatException)
            {
                MessageBox.Show("Error setting config values", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        public void TextChange(object sender, RoutedEventArgs e)
        {
            TextBox t = sender as TextBox;
            if (t != null)
            {
                t.FontStyle = FontStyles.Normal;
            }
        }

        private void posBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key.ToString() == "Return")
            {
                try
                {
                    int val = int.Parse(posBox.Text);
                    if (usb.SetPotValue(val))
                    {
                        potValuesBox.Items.Insert(0, String.Format("({0})", val));
                    }
                    else
                    {
                        MessageBox.Show("Error setting pot value", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    }
                }
                catch (FormatException)
                {
                    MessageBox.Show("Error setting pot value", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                }
            }
        }
    }

}
