using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

using FTD2XX_NET;

namespace Turnomatic_Loader
{

    class UsbConnection
    {
        private FTDI device = new FTDI();

        private short numSongs = 0;
        private short usedPages = 0;
        private short maxSongs = 0;
        private short totalPages = 0;

        private List<String> songNames = new List<String>();
        
        public short NumSongs {
            get {
                return numSongs;
            }
        }

        public short MaxSongs {
            get {
                return maxSongs;
            }
        }

        // Return used space in kB
        public double UsedSpace {
            get {
                return usedPages / 4.0;
            }
        }

        // Return total space in kB
        public double TotalSpace {
            get {
                return totalPages / 4.0;
            }
        }

        public List<String> SongNames {
            get {
                return songNames;
            }
        }

        public bool OpenDevice()
        {
            UInt32 ftdiDeviceCount = 0;
            FTDI.FT_STATUS ftStatus = FTDI.FT_STATUS.FT_OK;
            ftStatus = device.GetNumberOfDevices(ref ftdiDeviceCount);
            if (ftStatus != FTDI.FT_STATUS.FT_OK)
            {
                return false;
            }
            
            if (ftdiDeviceCount == 0)
            {
                return false;
            }

            // Allocate storage for device info list
            FTDI.FT_DEVICE_INFO_NODE[] ftdiDeviceList = new FTDI.FT_DEVICE_INFO_NODE[ftdiDeviceCount];

            // Populate our device list
            ftStatus = device.GetDeviceList(ftdiDeviceList);

            if (ftStatus != FTDI.FT_STATUS.FT_OK)
            {
                return false;
            }

            // Open first device in our list by serial number
            ftStatus = device.OpenBySerialNumber(ftdiDeviceList[0].SerialNumber);
            if (ftStatus != FTDI.FT_STATUS.FT_OK)
            {
                return false;
            }

            // Set up device data parameters
            // Set Baud rate to 115200
            ftStatus = device.SetBaudRate(115200);
            if (ftStatus != FTDI.FT_STATUS.FT_OK)
            {
                return false;
            }

            // Set data characteristics - Data bits, Stop bits, Parity
            ftStatus = device.SetDataCharacteristics(FTDI.FT_DATA_BITS.FT_BITS_8, FTDI.FT_STOP_BITS.FT_STOP_BITS_1, FTDI.FT_PARITY.FT_PARITY_NONE);
            if (ftStatus != FTDI.FT_STATUS.FT_OK)
            {
                return false;
            }

            // Set flow control - set RTS/CTS flow control
            ftStatus = device.SetFlowControl(FTDI.FT_FLOW_CONTROL.FT_FLOW_RTS_CTS, 0x11, 0x13);
            if (ftStatus != FTDI.FT_STATUS.FT_OK)
            {
                return false;
            }

            // Set read timeout to 5 seconds, write timeout to 5s
            ftStatus = device.SetTimeouts(5000, 5000);
            if (ftStatus != FTDI.FT_STATUS.FT_OK)
            {
                return false;
            }

            return true;
        }

        public bool CloseDevice()
        {
            FTDI.FT_STATUS ftStatus = device.Close();
            return (ftStatus == FTDI.FT_STATUS.FT_OK);
        }

        public bool GetSongList()
        {
            return true;
        }

        public bool AddSong(Song s, byte[] page_breaks, String name)
        {
            return true;
        }

        public bool DeleteSong(int num)
        {
            return true;
        }

    }

}
