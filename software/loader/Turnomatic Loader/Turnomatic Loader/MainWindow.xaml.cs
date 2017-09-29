using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Turnomatic_Loader
{

    public partial class MainWindow : Window
    {

        private Song song;
        private UsbConnection usb = new UsbConnection();
        private bool deviceConnected = false;
        private bool fileLoaded = false;

        private TextBox[] pageBreaks = null; 

        public MainWindow()
        {
            InitializeComponent();
            pageBreaks = new TextBox[]
            {
                break1,
                break2,
                break3,
                break4
            };
        }

        private void btn_browse(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "MIDI files (*.mid;*.midi)|*.mid;*.midi";
            if (openFileDialog.ShowDialog() == true)
            {
                String fileName = openFileDialog.FileName;
                if (fileName.EndsWith(".mid") || fileName.EndsWith(".midi"))
                {
                    try
                    {
                        song = new Song(fileName);
                    } catch (IOException)
                    {
                        MessageBox.Show("Error reading MIDI file", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                        fileLoaded = false;
                        fileNameBox.Text = "Select MIDI file";
                        songLengthBox.Text = "-";
                        songSizeBox.Text = "-";
                        songName.IsEnabled = false;
                        numPagesBox.IsEnabled = false;
                        for (int i = 0; i < 4; i++)
                        {
                            pageBreaks[i].IsEnabled = false;
                        }
                        return;
                    }

                    fileLoaded = true;
                    // Display filename in textbox
                    var fileUri = new Uri(fileName);
                    var cwdUri = new Uri(Directory.GetCurrentDirectory());
                    String relFileName = cwdUri.MakeRelativeUri(fileUri).ToString();
                    if (relFileName.Length < fileName.Length)
                    {
                        fileNameBox.Text = relFileName;
                    } else
                    {
                        fileNameBox.Text = fileName;
                    }
                    songLengthBox.Text = song.Length.ToString() + " bars";
                    songSizeBox.Text = song.Size.ToString("0.00") + " kB";
                    songName.IsEnabled = true;
                    numPagesBox.IsEnabled = true;
                    for (int i = 0; i < 4; i++)
                    {
                        pageBreaks[i].IsEnabled = (i < numPagesBox.SelectedIndex);
                    }
                }
            }
        }

        private void btn_scanDevice(object sender, RoutedEventArgs e)
        {
            if (deviceConnected)
            {
                usb.CloseDevice();
            }
            if (usb.OpenDevice())
            {
                deviceConnected = true;
                // Populate fields with data
                foundDevice.Text = "Device connected";
                if (usb.GetSongList())
                {
                    songListBox.ItemsSource = usb.SongNames;
                    numSongsBox.Text = usb.NumSongs.ToString();
                    maxSongsBox.Text = usb.MaxSongs.ToString();
                    usedSpaceBox.Text = usb.UsedSpace.ToString("0.00");
                    totalSpaceBox.Text = usb.UsedSpace.ToString("0.00");
                    // Activate song list box
                    songListBox.IsEnabled = true;
                }
            } else
            {
                deviceConnected = false;
                foundDevice.Text = "No devices found";
                songListBox.IsEnabled = false;
            }
            scanDevice.Content = "Rescan";
        }

        private void comb_sel(object sender, RoutedEventArgs e)
        {
            ComboBox comb = sender as ComboBox;
            if (comb != null && pageBreaks != null)
            {
                for (int i = 0; i < 4; i++)
                {
                    pageBreaks[i].IsEnabled = (i < comb.SelectedIndex);
                }
            }
        }

        private void DelSongCommand_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            // Can Execute as long as device is connected and song is selected
            e.CanExecute = deviceConnected && (songListBox.SelectedIndex != -1);
        }

        private void DelSongCommand_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            if (usb.DeleteSong(songListBox.SelectedIndex))
            {
                // Update fields
                if (usb.GetSongList())
                {
                    songListBox.ItemsSource = usb.SongNames;
                    numSongsBox.Text = usb.NumSongs.ToString();
                    maxSongsBox.Text = usb.MaxSongs.ToString();
                    usedSpaceBox.Text = usb.UsedSpace.ToString("0.00");
                    totalSpaceBox.Text = usb.UsedSpace.ToString("0.00");
                }
            }
            else
            {
                // Generate error message
                MessageBox.Show("Error deleting file", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void LoadSongCommand_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            bool canExecute = deviceConnected && fileLoaded && (songName.Text.Length > 0);
            if (canExecute)
            {
                int last = 0;
                int curr;
                // Ensure page numbers are filled and in order
                for (int i = 0; i < numPagesBox.SelectedIndex; i++)
                {
                    try
                    {
                        curr = Byte.Parse(pageBreaks[i].Text);
                    }
                    catch (FormatException)
                    {
                        canExecute = false;
                        break;
                    }
                    if (curr <= last)
                    {
                        canExecute = false;
                        break;
                    }
                    last = curr;
                }
            }
            e.CanExecute = canExecute;
        }

        private void LoadSongCommand_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            byte[] breaks = new byte[numPagesBox.SelectedIndex];
            for (int i = 0; i < numPagesBox.SelectedIndex; i++)
            {
                breaks[i] = Byte.Parse(pageBreaks[i].Text);
            }

            if (usb.AddSong(song, breaks, songName.Text))
            {
                // Update fields
                if (usb.GetSongList())
                {
                    songListBox.ItemsSource = usb.SongNames;
                    numSongsBox.Text = usb.NumSongs.ToString();
                    maxSongsBox.Text = usb.MaxSongs.ToString();
                    usedSpaceBox.Text = usb.UsedSpace.ToString("0.00");
                    totalSpaceBox.Text = usb.UsedSpace.ToString("0.00");
                }
            }
            else
            {
                // Generate error message
                MessageBox.Show("Error uploading file", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

    }

    public static class CustomCommands
    {
        public static readonly RoutedUICommand DelSong = new RoutedUICommand
            (
                "DelSong",
                "DelSong",
                typeof(CustomCommands),
                new InputGestureCollection()
                {
                    new KeyGesture(Key.D, ModifierKeys.Control)
                }
            );

        public static readonly RoutedUICommand LoadSong = new RoutedUICommand
            (
                "LoadSong",
                "LoadSong",
                typeof(CustomCommands),
                new InputGestureCollection()
                {
                    new KeyGesture(Key.L, ModifierKeys.Control)
                }
            );
    }

}
