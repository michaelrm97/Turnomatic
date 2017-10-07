using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

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
                    // Display filename in textbox
                    var fileUri = new Uri(fileName);
                    var referenceUri = new Uri(Directory.GetCurrentDirectory());
                    String relFileName = referenceUri.MakeRelativeUri(fileUri).ToString();
                    if (relFileName.StartsWith(".."))
                    {
                        fileNameBox.Text = fileName;
                    } else
                    {
                        fileNameBox.Text = relFileName;
                    }
                } else
                {
                    MessageBox.Show("Please specify a MIDI file", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                }
            }
        }

        private void btn_scanDevice(object sender, RoutedEventArgs e)
        {
            if (usb.IsOpen)
            {
                usb.CloseDevice();
            }
            if (usb.OpenDevice())
            {
                if (usb.GetSongList())
                {
                    deviceConnected = true;
                    foundDevice.Text = "Device connected";
                    songListBox.Items.Clear();
                    foreach (String name in usb.SongNames)
                    {
                        songListBox.Items.Add(name);
                    }
                    numSongsBox.Text = usb.NumSongs.ToString();
                    maxSongsBox.Text = usb.MaxSongs.ToString();
                    usedSpaceBox.Text = usb.UsedSpace.ToString("0.00");
                    totalSpaceBox.Text = usb.TotalSpace.ToString("0.00");
                    // Activate song list box
                    songListBox.IsEnabled = true;
                } else
                {
                    foundDevice.Text = "Error connecting";
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

        private void btn_loadSong(object sender, RoutedEventArgs e)
        {
            String fileName = fileNameBox.Text;
            if (fileName.Length == 0)
            {
                // Throw error message
                MessageBox.Show("Please specify a filename", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            if (!(fileName.EndsWith(".mid") || fileName.EndsWith(".midi")))
            {
                // Throw error message
                MessageBox.Show("Please specify a MIDI file", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            try
            {

                Regex rgx1 = new Regex(@"(?<=\[)\d+ \d+(?=\])");

                List<int[]> repeats = new List<int[]>();
                foreach (Match m in rgx1.Matches(repeatsList.Text))
                {
                    int[] bars = new int[2];
                    String[] barStrings = m.ToString().Split(' ');
                    Debug.WriteLine(barStrings);
                    bars[0] = int.Parse(barStrings[0]);
                    bars[1] = int.Parse(barStrings[1]);
                    if (bars[1] < bars[0])
                    {
                        MessageBox.Show("Invalid repeats", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                        return;
                    }
                    repeats.Add(bars);
                }
                song = new Song(fileNameBox.Text, repeats);
            }
            catch (IOException)
            {
                MessageBox.Show("Could not open MIDI file", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }
            catch (FileFormatException)
            {
                MessageBox.Show("Error reading MIDI file", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            songLengthBox.Text = song.Length.ToString() + " bars";
            songSizeBox.Text = song.SizekB.ToString("0.00") + " kB";
            songName.IsEnabled = true;
            numPagesBox.IsEnabled = true;
            for (int i = 0; i < 4; i++)
            {
                pageBreaks[i].IsEnabled = (i < numPagesBox.SelectedIndex);
            }

            fileLoaded = true;
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
                    songListBox.Items.Clear();
                    foreach (String name in usb.SongNames)
                    {
                        songListBox.Items.Add(name);
                    }
                    numSongsBox.Text = usb.NumSongs.ToString();
                    maxSongsBox.Text = usb.MaxSongs.ToString();
                    usedSpaceBox.Text = usb.UsedSpace.ToString("0.00");
                    totalSpaceBox.Text = usb.TotalSpace.ToString("0.00");
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
                        curr = short.Parse(pageBreaks[i].Text);
                    }
                    catch (FormatException)
                    {
                        canExecute = false;
                        break;
                    } catch (OverflowException)
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
            // Check we have enough space first
            if (usb.NumSongs >= usb.MaxSongs)
            {
                MessageBox.Show("Maximum number of songs reached", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }
            if (usb.UsedSpace + song.SizekB > usb.TotalSpace)
            {
                MessageBox.Show("Insufficient space available", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            short[] breaks = new short[numPagesBox.SelectedIndex];
            for (int i = 0; i < numPagesBox.SelectedIndex; i++)
            {
                breaks[i] = short.Parse(pageBreaks[i].Text);
            }

            if (usb.AddSong(song, breaks, songName.Text))
            {
                // Update fields
                if (usb.GetSongList())
                {
                    songListBox.Items.Clear();
                    foreach (String name in usb.SongNames)
                    {
                        songListBox.Items.Add(name);
                    }
                    numSongsBox.Text = usb.NumSongs.ToString();
                    maxSongsBox.Text = usb.MaxSongs.ToString();
                    usedSpaceBox.Text = usb.UsedSpace.ToString("0.00");
                    totalSpaceBox.Text = usb.TotalSpace.ToString("0.00");
                }
            }
            else
            {
                // Generate error message
                MessageBox.Show("Error uploading file", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void ConfigDeviceCommand_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            // Can Execute as long as device is connected and song is selected
            e.CanExecute = deviceConnected;
        }

        private void ConfigDeviceCommand_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            ConfigDialogBox dlg = new ConfigDialogBox();

            // Configure the dialog box
            dlg.Owner = this;
            dlg.Usb = usb;

            // Open the dialog box modally 
            dlg.ShowDialog();


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

        public static readonly RoutedUICommand ConfigDevice = new RoutedUICommand
            (
                "ConfigDevice",
                "ConfigDevice",
                typeof(CustomCommands),
                new InputGestureCollection()
                {
                    new KeyGesture(Key.Q, ModifierKeys.Control | ModifierKeys.Shift)
                }
            );
    }

}
