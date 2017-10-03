using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

using FTD2XX_NET;
using System.Linq;

namespace Turnomatic_Loader
{

    public class UsbConnection
    {

        private const UInt32 TIMEOUT = 100;

        private FTDI device = new FTDI();

        private short numSongs = 0;
        private short usedPages = 0;
        private short maxSongs = 0;
        private short totalPages = 0;

        private List<String> songNames = new List<String>();

        private int potValue = 0;

        private int page1pos = 0;
        private int pagePosDiff = 0;
        private float motorGain = 0;
        private float micThresh = 0;
        
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

        public bool IsOpen {
            get {
                return device.IsOpen;
            }
        }

        public int PotValue {
            get {
                return potValue;
            }
        }

        public int Page1Pos {
            get {
                return page1pos;
            }
        }

        public int PagePosDiff {
            get {
                return pagePosDiff;
            }
        }

        public float MotorGain {
            get {
                return motorGain;
            }
        }

        public float MicThresh {
            get {
                return micThresh;
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

            FTDI.FT_STATUS ftStatus;
            int timeout;

            byte[] command = Encoding.ASCII.GetBytes("LIST");
            byte[] numCode = BitConverter.GetBytes(0);
            byte[] writeData = new byte[8];
            command.CopyTo(writeData, 0);
            numCode.CopyTo(writeData, 4);
            UInt32 numBytesWritten = 0;

            // Write data to device
            ftStatus = device.Write(writeData, writeData.Length, ref numBytesWritten);
            if (ftStatus != FTDI.FT_STATUS.FT_OK)
            {
                return false;
            }

            UInt32 numBytesAvailable = 0;
            timeout = 0;
            do
            {
                if (timeout++ == TIMEOUT)
                {
                    return false;
                }
                ftStatus = device.GetRxBytesAvailable(ref numBytesAvailable);
                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                {
                    return false;
                }
                Thread.Sleep(10);
            } while (numBytesAvailable < 8);

            byte[] readData = new byte[8];
            UInt32 numBytesRead = 0;
            // Read first 8 bytes to determine how many song names to expect
            ftStatus = device.Read(readData, 8, ref numBytesRead);

            numSongs = (short) (readData[0] + (readData[1] << 8));
            usedPages = (short)(readData[2] + (readData[3] << 8));
            maxSongs = (short)(readData[4] + (readData[5] << 8));
            totalPages = (short)(readData[6] + (readData[7] << 8));

            songNames.Clear();

            // Read 16 songs at a time
            UInt32 numSongsRead = 0;
            while (numSongsRead < numSongs)
            {
                UInt32 songsToRead = (UInt32) Math.Min(16, numSongs - numSongsRead);
                readData = new byte[songsToRead * 20];
                timeout = 0;
                do
                {
                    if (timeout++ == TIMEOUT)
                    {
                        return false;
                    }
                    ftStatus = device.GetRxBytesAvailable(ref numBytesAvailable);
                    if (ftStatus != FTDI.FT_STATUS.FT_OK)
                    {
                        return false;
                    }
                    Thread.Sleep(10);
                } while (numBytesAvailable < 20 * songsToRead);

                ftStatus = device.Read(readData, 20 * songsToRead, ref numBytesRead);
                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                {
                    return false;
                }

                for (UInt32 i = 0; i < songsToRead; i++)
                {
                    byte[] nameBytes = new byte[20];
                    Array.Copy(readData, i * 20, nameBytes, 0, 20);
                    songNames.Add(Encoding.ASCII.GetString(nameBytes).TrimEnd('\0'));
                }

                numSongsRead += songsToRead;
            }

            return true;
        }

        public bool AddSong(Song s, short[] pageBreaks, String name)
        {
            FTDI.FT_STATUS ftStatus;
            int timeout;

            byte[] command = Encoding.ASCII.GetBytes("ADDS");
            byte[] numCode = BitConverter.GetBytes(s.sizeBytes);
            byte[] writeData = new byte[8];
            command.CopyTo(writeData, 0);
            numCode.CopyTo(writeData, 4);
            UInt32 numBytesWritten = 0;

            // Write data to device
            ftStatus = device.Write(writeData, writeData.Length, ref numBytesWritten);
            if (ftStatus != FTDI.FT_STATUS.FT_OK)
            {
                return false;
            }

            UInt32 numBytesAvailable = 0;
            timeout = 0;
            do
            {
                if (timeout++ == TIMEOUT)
                {
                    return false;
                }
                ftStatus = device.GetRxBytesAvailable(ref numBytesAvailable);
                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                {
                    return false;
                }
                Thread.Sleep(10);
            } while (numBytesAvailable < 4);

            byte[] readData = new byte[4];
            UInt32 numBytesRead = 0;
            // Read first 4 bytes to determine if request is successful
            ftStatus = device.Read(readData, 4, ref numBytesRead);
            String succ = Encoding.ASCII.GetString(readData);
            if (succ != "SUCC")
            {
                return false;
            }

            // Write song metadata
            writeData = s.GetInfo(pageBreaks, name);
            ftStatus = device.Write(writeData, writeData.Length, ref numBytesWritten);
            if (ftStatus != FTDI.FT_STATUS.FT_OK)
            {
                return false;
            }

            writeData = s.GetData();
            for (int i = 0; i < writeData.Length; i += 256)
            {
                // Wait for ack
                timeout = 0;
                do
                {
                    if (timeout++ == TIMEOUT)
                    {
                        return false;
                    }
                    ftStatus = device.GetRxBytesAvailable(ref numBytesAvailable);
                    if (ftStatus != FTDI.FT_STATUS.FT_OK)
                    {
                        return false;
                    }
                    Thread.Sleep(10);
                } while (numBytesAvailable < 4);
                ftStatus = device.Read(readData, 4, ref numBytesRead);
                String ackk = Encoding.ASCII.GetString(readData);
                if (ackk != "ACKK")
                {
                    return false;
                }

                int num = Math.Min(256, writeData.Length - i);

                byte[] toWrite = writeData.Skip(i).Take(num).ToArray();

                ftStatus = device.Write(toWrite, num, ref numBytesWritten);
                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                {
                    return false;
                }
            }

            timeout = 0;
            do
            {
                if (timeout++ == TIMEOUT)
                {
                    return false;
                }
                ftStatus = device.GetRxBytesAvailable(ref numBytesAvailable);
                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                {
                    return false;
                }
                Thread.Sleep(10);
            } while (numBytesAvailable < 4);

            readData = new byte[4];
            // Read first 4 bytes to determine if request is successful
            ftStatus = device.Read(readData, 4, ref numBytesRead);
            if (ftStatus != FTDI.FT_STATUS.FT_OK)
            {
                return false;
            }

            succ = Encoding.ASCII.GetString(readData);
            if (succ != "SUCC")
            {
                return false;
            }

            return true;
        }

        public bool DeleteSong(int num)
        {
            FTDI.FT_STATUS ftStatus;
            UInt32 timeout;

            byte[] command = Encoding.ASCII.GetBytes("DELS");
            byte[] numCode = BitConverter.GetBytes(num);
            byte[] writeData = new byte[8];
            command.CopyTo(writeData, 0);
            numCode.CopyTo(writeData, 4);
            UInt32 numBytesWritten = 0;

            // Write data to device
            ftStatus = device.Write(writeData, writeData.Length, ref numBytesWritten);
            if (ftStatus != FTDI.FT_STATUS.FT_OK)
            {
                return false;
            }

            while (true)
            {
                UInt32 numBytesAvailable = 0;
                timeout = 0;
                do
                {
                    if (timeout++ == TIMEOUT)
                    {
                        return false;
                    }
                    ftStatus = device.GetRxBytesAvailable(ref numBytesAvailable);
                    if (ftStatus != FTDI.FT_STATUS.FT_OK)
                    {
                        return false;
                    }
                    Thread.Sleep(10);
                } while (numBytesAvailable < 4);

                byte[] readData = new byte[4];
                UInt32 numBytesRead = 0;
                ftStatus = device.Read(readData, 4, ref numBytesRead);
                String succ = Encoding.ASCII.GetString(readData);
                if (succ == "ACKK")
                {
                    continue;
                } else if (succ == "SUCC")
                {
                    return true;
                } else
                {
                    return false;
                }

            }

        }

        public bool GetPotValue()
        {
            FTDI.FT_STATUS ftStatus;
            int timeout;

            byte[] command = Encoding.ASCII.GetBytes("POTV");
            byte[] numCode = BitConverter.GetBytes(0);
            byte[] writeData = new byte[8];
            command.CopyTo(writeData, 0);
            numCode.CopyTo(writeData, 4);
            UInt32 numBytesWritten = 0;

            // Write data to device
            ftStatus = device.Write(writeData, writeData.Length, ref numBytesWritten);
            if (ftStatus != FTDI.FT_STATUS.FT_OK)
            {
                return false;
            }

            UInt32 numBytesAvailable = 0;
            timeout = 0;
            do
            {
                if (timeout++ == TIMEOUT)
                {
                    return false;
                }
                ftStatus = device.GetRxBytesAvailable(ref numBytesAvailable);
                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                {
                    return false;
                }
                Thread.Sleep(10);
            } while (numBytesAvailable < 4);

            byte[] readData = new byte[4];
            UInt32 numBytesRead = 0;

            ftStatus = device.Read(readData, 4, ref numBytesRead);
            if (ftStatus != FTDI.FT_STATUS.FT_OK)
            {
                return false;
            }

            potValue = BitConverter.ToInt32(readData, 0);

            return true;

        }

        public bool SetPotValue(int val)
        {
            FTDI.FT_STATUS ftStatus;
            int timeout;

            byte[] command = Encoding.ASCII.GetBytes("POTS");
            byte[] numCode = BitConverter.GetBytes(val);
            byte[] writeData = new byte[8];
            command.CopyTo(writeData, 0);
            numCode.CopyTo(writeData, 4);
            UInt32 numBytesWritten = 0;

            // Write data to device
            ftStatus = device.Write(writeData, writeData.Length, ref numBytesWritten);
            if (ftStatus != FTDI.FT_STATUS.FT_OK)
            {
                return false;
            }

            UInt32 numBytesAvailable = 0;
            timeout = 0;
            do
            {
                if (timeout++ == TIMEOUT)
                {
                    return false;
                }
                ftStatus = device.GetRxBytesAvailable(ref numBytesAvailable);
                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                {
                    return false;
                }
                Thread.Sleep(10);
            } while (numBytesAvailable < 4);

            byte[] readData = new byte[4];
            UInt32 numBytesRead = 0;

            ftStatus = device.Read(readData, 4, ref numBytesRead);
            if (ftStatus != FTDI.FT_STATUS.FT_OK)
            {
                return false;
            }

            String succ = Encoding.ASCII.GetString(readData);
            if (succ != "SUCC")
            {
                return false;
            }

            return true;

        }

        public bool GetConfigValues()
        {
            FTDI.FT_STATUS ftStatus;
            int timeout;

            byte[] command = Encoding.ASCII.GetBytes("CFGV");
            byte[] numCode = BitConverter.GetBytes(0);
            byte[] writeData = new byte[8];
            command.CopyTo(writeData, 0);
            numCode.CopyTo(writeData, 4);
            UInt32 numBytesWritten = 0;

            // Write data to device
            ftStatus = device.Write(writeData, writeData.Length, ref numBytesWritten);
            if (ftStatus != FTDI.FT_STATUS.FT_OK)
            {
                return false;
            }

            UInt32 numBytesAvailable = 0;
            timeout = 0;
            do
            {
                if (timeout++ == TIMEOUT)
                {
                    return false;
                }
                ftStatus = device.GetRxBytesAvailable(ref numBytesAvailable);
                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                {
                    return false;
                }
                Thread.Sleep(10);
            } while (numBytesAvailable < 16);

            byte[] readData = new byte[16];
            UInt32 numBytesRead = 0;

            ftStatus = device.Read(readData, 16, ref numBytesRead);
            if (ftStatus != FTDI.FT_STATUS.FT_OK)
            {
                return false;
            }

            page1pos = BitConverter.ToInt32(readData, 0);
            pagePosDiff = BitConverter.ToInt32(readData, 4);
            motorGain = BitConverter.ToSingle(readData, 8);
            micThresh = BitConverter.ToSingle(readData, 12);

            return true;

        }

        public bool SetConfigValues(int _page1pos, int _pagePosDiff, float _motorGain, float _micThresh)
        {
            FTDI.FT_STATUS ftStatus;
            int timeout;

            byte[] command = Encoding.ASCII.GetBytes("CFGS");
            byte[] numCode = BitConverter.GetBytes(0);
            byte[] writeData = new byte[8];
            command.CopyTo(writeData, 0);
            numCode.CopyTo(writeData, 4);
            UInt32 numBytesWritten = 0;

            // Write data to device
            ftStatus = device.Write(writeData, writeData.Length, ref numBytesWritten);
            if (ftStatus != FTDI.FT_STATUS.FT_OK)
            {
                return false;
            }

            UInt32 numBytesAvailable = 0;
            timeout = 0;
            do
            {
                if (timeout++ == TIMEOUT)
                {
                    return false;
                }
                ftStatus = device.GetRxBytesAvailable(ref numBytesAvailable);
                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                {
                    return false;
                }
                Thread.Sleep(10);
            } while (numBytesAvailable < 4);

            byte[] readData = new byte[4];
            UInt32 numBytesRead = 0;

            ftStatus = device.Read(readData, 4, ref numBytesRead);
            if (ftStatus != FTDI.FT_STATUS.FT_OK)
            {
                return false;
            }

            String succ = Encoding.ASCII.GetString(readData);
            if (succ != "SUCC")
            {
                return false;
            }

            writeData = BitConverter.GetBytes(_page1pos).Concat(BitConverter.GetBytes(_pagePosDiff)).Concat(BitConverter.GetBytes(_motorGain)).Concat(BitConverter.GetBytes(_micThresh)).ToArray();

            // Write data to device
            ftStatus = device.Write(writeData, writeData.Length, ref numBytesWritten);
            if (ftStatus != FTDI.FT_STATUS.FT_OK)
            {
                return false;
            }

            numBytesAvailable = 0;
            timeout = 0;
            do
            {
                if (timeout++ == TIMEOUT)
                {
                    return false;
                }
                ftStatus = device.GetRxBytesAvailable(ref numBytesAvailable);
                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                {
                    return false;
                }
                Thread.Sleep(10);
            } while (numBytesAvailable < 4);

            readData = new byte[4];
            numBytesRead = 0;

            ftStatus = device.Read(readData, 4, ref numBytesRead);
            if (ftStatus != FTDI.FT_STATUS.FT_OK)
            {
                return false;
            }

            succ = Encoding.ASCII.GetString(readData);
            if (succ != "SUCC")
            {
                return false;
            }

            return true;

        }

    }

}
