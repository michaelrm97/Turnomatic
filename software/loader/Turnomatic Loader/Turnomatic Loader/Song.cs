using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Turnomatic_Loader
{

    class Song
    {
        internal struct Chord
        {
            internal byte[] notes;
            internal byte dur;
            internal short bar;

            internal Chord(byte[] _notes, byte _dur, short _bar)
            {
                notes = _notes;
                dur = _dur;
                bar = _bar;
            }
        }

        internal class KeySignature
        {
            private sbyte sharps;
            private bool major;

            internal KeySignature(sbyte _sharps, bool _major)
            {
                sharps = _sharps;
                major = _major;
            }

            internal sbyte Sharps {
                get {
                    return sharps;
                }
            }

            internal bool Major {
                get {
                    return major;
                }
            }

            internal String GetString {
                get {
                    String[] keyStrings = {
                        "Cb", "Gb", "Db", "Ab", "Eb", "Bb", "F", "C", "G", "D", "A", "E", "B", "F#", "C#"
                    };
                    if (major)
                    {
                        return keyStrings[sharps + 7];
                    } else
                    {
                        return keyStrings[(sharps + 10) % 15];
                    }
                }
            }

        }

        private List<Chord> chords = new List<Chord>();

        public int Length {
            get {
                if (chords.Count > 0) {
                    return chords[chords.Count - 1].bar; // Number of bars
                } else
                {
                    return 0;
                }
            }
        }

        public double SizekB {
            get {
                int numPages = (chords.Count + 31) / 32;
                return numPages / 4.0; // Size in kB rounded up to nearest 256 bytes
            }
        }

        public int sizeBytes {
            get {
                return chords.Count * 8;
            }
        } 

        // Convert a midi file to a song
        public Song(String fileName)
        {
            chords.Clear();

            BinaryReader br;
            try
            {
                br = new BinaryReader(new FileStream(fileName, FileMode.Open));
            }
            catch (IOException e)
            {
                throw e;
            }

            // Read header
            byte[] header = br.ReadBytes(14);
            String mthd = Encoding.ASCII.GetString(header, 0, 4);
            int headerLength = BitConverter.ToInt32(header.Skip(4).Take(4).Reverse().ToArray(), 0);
            short format = BitConverter.ToInt16(header.Skip(8).Take(2).Reverse().ToArray(), 0);
            short n = BitConverter.ToInt16(header.Skip(10).Take(2).Reverse().ToArray(), 0);
            short division = BitConverter.ToInt16(header.Skip(12).Take(2).Reverse().ToArray(), 0);

            Debug.WriteLine(mthd);
            Debug.WriteLine(headerLength);
            Debug.WriteLine(format);
            Debug.WriteLine(n);
            Debug.WriteLine(division);
            
            if (mthd != "MThd" || headerLength != 6)
            {
                throw new FileFormatException();
            }

            // Only read first chunk
            byte[] chunkHeader = br.ReadBytes(8);
            String mtrk = Encoding.ASCII.GetString(chunkHeader, 0, 4);
            int chunkLength = BitConverter.ToInt32(chunkHeader.Skip(4).Take(4).Reverse().ToArray(), 0);

            Debug.WriteLine(mtrk);
            Debug.WriteLine(chunkLength);

            if (mtrk != "MTrk")
            {
                throw new FileFormatException();
            }

            byte[] chunk = br.ReadBytes(chunkLength);
            int offset = 0;
            bool noteOn = false;
            bool endOfTrack = false;

            int tempo = 0; // microseconds per quarter note
            int timeSig = 0; // quarter notes per bar
            KeySignature keySig = new KeySignature(0, true); // Initally C Major

            byte[] noteValues = new byte[128];
            byte[] oldValues = new byte[128];

            int reftime = 0; // Time when current time signature first started
            int ctime = 0; // Current time of track
            int ltime = 0; // Time of last change

            int refbar = 1;
            int cbar = 1;

            bool notesChanged = false;

            while (offset < chunkLength && !endOfTrack)
            {
                int delta = GetVarLength(chunk, ref offset);
                if (delta > 0 && notesChanged)
                {
                    
                    // Create a chord out of currently played notes
                    int dtime = ctime - ltime;
                    byte dur = (byte)Math.Round(4 * (double)dtime / division);
                    Debug.WriteLine("ctime: {0} ltime: {1}", ctime, ltime);
                    if (dur > 0)
                    {
                        short bar = (short)(((ltime - reftime) / (division * timeSig)) + refbar);
                        byte[] notes = oldValues.Select((s, i) => new { i, s }).Where(t => t.s > 0).Select(t => (byte) t.i).ToArray();
                        Debug.WriteLine(String.Format("Dur: {0} Bar: {1} Notes: {2}", dur, bar, BitConverter.ToString(notes)));
                        if (notes.Length > 0)
                        {
                            chords.Add(new Chord(notes, dur, bar));
                        }
                    }

                    Array.Copy(noteValues, oldValues, 128);

                    notesChanged = false;
                    ltime = ctime;
                }
                ctime += delta;
                if (division != 0 && timeSig != 0)
                {
                    cbar = (int)((ctime - reftime) / (division * timeSig)) + refbar;
                    Debug.WriteLine(String.Format("Current time: {0} Current bar: {1}", ctime, cbar));
                }

                // Debug.WriteLine(String.Format("Time delta: {0}", delta));
                byte message = chunk[offset++];
                if (message == 0xFF)
                {
                    // Meta event
                    byte metaType = chunk[offset++];
                    int vlen = GetVarLength(chunk, ref offset);
                    byte[] vData = chunk.Skip(offset).Take(vlen).ToArray();
                    offset += vlen;
                    switch (metaType) {
                        case 0x00:
                            // Sequence number
                            short seqNum = BitConverter.ToInt16(vData.Reverse().ToArray(), 0);
                            Debug.WriteLine(String.Format("Sequence number: {0}", seqNum));
                            ltime = ctime;
                            break;
                        case 0x01:
                            // Text event
                            Debug.WriteLine(String.Format("Text: {0}", Encoding.ASCII.GetString(vData)));
                            break;
                        case 0x02:
                            // Copyright notice
                            Debug.WriteLine(String.Format("Copyright: {0}", Encoding.ASCII.GetString(vData)));
                            break;
                        case 0x03:
                            // Sequence or track name
                            Debug.WriteLine(String.Format("Sequence Name: {0}", Encoding.ASCII.GetString(vData)));
                            break;
                        case 0x04:
                            // Instrument name
                            Debug.WriteLine(String.Format("Instrument Name: {0}", Encoding.ASCII.GetString(vData)));
                            break;
                        case 0x05:
                            // Lyric text
                            Debug.WriteLine(String.Format("Lyric: {0}", Encoding.ASCII.GetString(vData)));
                            break;
                        case 0x06:
                            // Marker text
                            Debug.WriteLine(String.Format("Marker: {0}", Encoding.ASCII.GetString(vData)));
                            break;
                        case 0x07:
                            // Cue point
                            Debug.WriteLine(String.Format("Cue Point: {0}", Encoding.ASCII.GetString(vData)));
                            break;
                        case 0x20:
                            // MIDI channel prefix assignment
                            byte channel = vData[0];
                            Debug.WriteLine(String.Format("MIDI Channel Prefix: {0}", channel));
                            break;
                        case 0x2F:
                            // End of track
                            endOfTrack = true;
                            Debug.WriteLine("End of track");
                            break;
                        case 0x51:
                            // Tempo setting
                            tempo = BitConverter.ToInt32(vData.Reverse().Concat(new byte[] { 0 }).ToArray(), 0);
                            Debug.WriteLine(String.Format("Tempo: {0}", tempo));
                            break;
                        case 0x54:
                            // SMPTE offset
                            byte hr = vData[0];
                            byte mn = vData[1];
                            byte se = vData[2];
                            byte fr = vData[3];
                            byte ff = vData[4];
                            Debug.WriteLine(String.Format("SMPTE offset: hr:{0} mn:{1} se:{2} fr:{3} ff:{4}", hr, mn, se, fr, ff));
                            break;
                        case 0x58:
                            // Time signature
                            byte nn = vData[0];
                            byte dd = vData[1];
                            byte cc = vData[2];
                            byte bb = vData[3];
                            Debug.WriteLine(String.Format("Time signature: {0}/{1}", nn, Math.Pow(2, dd)));
                            Debug.WriteLine(String.Format("MIDI clocks per metronome tick: {0}", cc));
                            Debug.WriteLine(String.Format("32nd notes per MIDI quarter note: {0}", bb));
                            timeSig = (int) (nn * Math.Pow(2, 2 - dd));
                            reftime = ctime;
                            refbar = cbar;
                            break;
                        case 0x59:
                            sbyte sf = (sbyte)vData[0];
                            bool major = (vData[1] == 0);
                            // Key signature
                            keySig = new KeySignature(sf, major);
                            Debug.WriteLine(String.Format("Key signature: {0}", keySig.GetString));
                            break;
                        case 0x7F:
                            // Sequencer specific event
                            Debug.WriteLine(String.Format("Sequence specifier: {0}", BitConverter.ToString(vData)));
                            break;
                        default:
                            // Unknown event type
                            Debug.WriteLine("Unknown event type: {0} Len: {1} Data: {2}", metaType, vlen, BitConverter.ToString(vData));
                            break;
                    }

                } else if (message == 0xF0 || message == 0xF7) 
                {
                    // Sysex event
                    int vlen = GetVarLength(chunk, ref offset);
                    byte[] vData = chunk.Skip(offset).Take(vlen).ToArray();
                    offset += vlen;
                    Debug.WriteLine("Sysex event: Len: {0} Data: {1}", vlen, BitConverter.ToString(vData));
                } else
                {
                    byte status = (byte)(message & 0xF0);
                    byte channel = (byte)(message & 0x0F);
                    byte key, val;
                    switch(status)
                    {
                        case 0x80:
                            // Note off event
                            noteOn = false;
                            key = (byte)(chunk[offset++] & 0x7F);
                            val = (byte)(chunk[offset++] & 0x7F);
                            Debug.WriteLine("Note off: Key: {0} Vol: {1}", key, val);
                            noteValues[key] = 0;
                            notesChanged = true;
                            break;
                        case 0x90:
                            // Note on event
                            noteOn = true;
                            key = (byte)(chunk[offset++] & 0x7F);
                            val = (byte)(chunk[offset++] & 0x7F);
                            Debug.WriteLine("Note on: Key: {0} Vl: {1}", key, val);
                            noteValues[key] = val;
                            notesChanged = true;
                            break;
                        case 0xA0:
                            noteOn = false;
                            // Polyphonic key pressure
                            key = (byte)(chunk[offset++] & 0x7F);
                            val = (byte)(chunk[offset++] & 0x7F);
                            Debug.WriteLine("Polyphonic key pressure: Key: {0} Pressure value: {1}", key, val);
                            break;
                        case 0xB0:
                            noteOn = false;
                            // Control change
                            byte controller = (byte)(chunk[offset++] & 0x7F);
                            val = (byte)(chunk[offset++] & 0x7F);
                            Debug.WriteLine("Control change: Controller number: {0} Vel: {1}", controller, val);
                            break;
                        case 0xC0:
                            noteOn = false;
                            // Program change
                            byte program = (byte)(chunk[offset++] & 0x7F);
                            Debug.WriteLine("Program change: {0}", program);
                            break;
                        case 0xD0:
                            noteOn = false;
                            // Channel pressure
                            val = (byte)(chunk[offset++] & 0x7F);
                            Debug.WriteLine("Channel pressure: {0}", val);
                            break;
                        case 0xE0:
                            noteOn = false;
                            // Pitch Wheel Change
                            short wheel = (short)((chunk[offset++] & 0x7F) | ((chunk[offset++] & 0x7F) << 7));
                            Debug.WriteLine("Pitch wheel change: {0}", wheel);
                            break;
                        default:
                            if (noteOn)
                            {
                                // Part of note on event
                                key = (byte)(message & 0x7F);
                                val = (byte)(chunk[offset++] & 0x7F);
                                Debug.WriteLine("Update note value: Key: {0} Vel: {1}", key, val);
                                noteValues[key] = val;
                                notesChanged = true;
                            } else
                            {
                                // Unknown message type
                                Debug.WriteLine("Unknown message type: {0} Data: {1}", message, chunk[offset++]);
                            }
                            break;
                    }
                }

            }

            br.Close();
        }

        public byte[] GetInfo(short[] pageBreaks, String name)
        {
            byte[] data = new byte[32];
            byte[] numChords = BitConverter.GetBytes(chords.Count);
            byte[] pageBreakBytes = new byte[8];
            for (int i = 0; i < pageBreaks.Length; i++)
            {
                pageBreakBytes[2 * i] = (byte)(pageBreaks[i] & 0xFF);
                pageBreakBytes[2 * i + 1] = (byte)(pageBreaks[i] >> 8);
            }
            byte[] nameBytes = System.Text.Encoding.ASCII.GetBytes(name);

            numChords.CopyTo(data, 0);
            pageBreakBytes.CopyTo(data, 4);
            nameBytes.CopyTo(data, 12);

            return data;
        }

        public byte[] GetData()
        {
            byte[] data = new byte[8 * chords.Count];

            for (int i = 0; i < chords.Count; i++)
            {
                Chord chord = chords[i];
                for (int j = 0; j < chord.notes.Length; j++)
                {
                    data[8 * i + j] = chord.notes[j];
                }
                data[8 * i + 4] = (byte) chord.notes.Length;
                data[8 * i + 5] = chord.dur;
                data[8 * i + 6] = (byte)(chord.bar & 0xFF);
                data[8 * i + 7] = (byte)(chord.bar >> 8);
            }

            return data;
        }

        // Returns value directly, offset by reference
        static private int GetVarLength(byte[] data, ref int offset)
        {
            int result = 0;
            while (true)
            {
                byte val = data[offset++];
                result = (result << 7) | (val & 0x7F);
                if ((val & (1 << 7)) == 0)
                {
                    return result;
                }
            }
        }

    }
}
