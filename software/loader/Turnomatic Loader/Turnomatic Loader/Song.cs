using System;
using System.Collections.Generic;
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

            internal Chord(byte[] notes1, byte dur1, byte bar1)
            {
                notes = notes1;
                dur = dur1;
                bar = bar1;
            }
        }

        private List<Chord> chords = new List<Chord>();

        public int Length {
            get {
                return chords[chords.Count - 1].bar; // Number of bars
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
            // Dummy song (First 4 bars of Clocks)
            chords.Clear();
            chords.Add(new Chord(new Byte[] { 75 }, 4, 1));
            chords.Add(new Chord(new Byte[] { 70 }, 4, 1));
            chords.Add(new Chord(new Byte[] { 67 }, 4, 1));
            chords.Add(new Chord(new Byte[] { 75 }, 4, 1));
            chords.Add(new Chord(new Byte[] { 70 }, 4, 1));
            chords.Add(new Chord(new Byte[] { 67 }, 4, 1));
            chords.Add(new Chord(new Byte[] { 75 }, 4, 1));
            chords.Add(new Chord(new Byte[] { 70 }, 4, 1));
            chords.Add(new Chord(new Byte[] { 73 }, 4, 2));
            chords.Add(new Chord(new Byte[] { 70 }, 4, 2));
            chords.Add(new Chord(new Byte[] { 65 }, 4, 2));
            chords.Add(new Chord(new Byte[] { 73 }, 4, 2));
            chords.Add(new Chord(new Byte[] { 70 }, 4, 2));
            chords.Add(new Chord(new Byte[] { 65 }, 4, 2));
            chords.Add(new Chord(new Byte[] { 73 }, 4, 2));
            chords.Add(new Chord(new Byte[] { 70 }, 4, 2));
            chords.Add(new Chord(new Byte[] { 73 }, 4, 3));
            chords.Add(new Chord(new Byte[] { 70 }, 4, 3));
            chords.Add(new Chord(new Byte[] { 65 }, 4, 3));
            chords.Add(new Chord(new Byte[] { 73 }, 4, 3));
            chords.Add(new Chord(new Byte[] { 70 }, 4, 3));
            chords.Add(new Chord(new Byte[] { 65 }, 4, 3));
            chords.Add(new Chord(new Byte[] { 73 }, 4, 3));
            chords.Add(new Chord(new Byte[] { 70 }, 4, 3));
            chords.Add(new Chord(new Byte[] { 72 }, 4, 4));
            chords.Add(new Chord(new Byte[] { 68 }, 4, 4));
            chords.Add(new Chord(new Byte[] { 65 }, 4, 4));
            chords.Add(new Chord(new Byte[] { 72 }, 4, 4));
            chords.Add(new Chord(new Byte[] { 68 }, 4, 4));
            chords.Add(new Chord(new Byte[] { 65 }, 4, 4));
            chords.Add(new Chord(new Byte[] { 72 }, 4, 4));
            chords.Add(new Chord(new Byte[] { 68 }, 4, 4));

            //BinaryReader br;
            //try
            //{
            //    br = new BinaryReader(new FileStream(fileName, FileMode.Open));
            //} catch (IOException e)
            //{
            //    throw e;
            //}

            //throw new IOException("Not sure how to read midi files yet");

            //br.Close();
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

    }
}
