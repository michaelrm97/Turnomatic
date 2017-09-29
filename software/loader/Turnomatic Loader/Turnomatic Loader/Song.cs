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

        private List<Chord> chords;

        public int Length {
            get {
                return 23;
                //return chords[chords.Count - 1].bar; // Number of bars
            }
        }

        public double Size {
            get {
                return 4;
                //int numPages = (chords.Count + 31) / 32;
                //return numPages / 4; // Size in kB rounded up to nearest 256 bytes
            }
        }

        // Convert a midi file to a song
        public Song(String fileName)
        {
            // Dummy song (First bar of twinkle twinkle little star)
            //chords = new List<Chord>();
            //chords.Add(new Chord(new Byte[] { 69 }, 4, 1));
            //chords.Add(new Chord(new Byte[] { 69 }, 4, 1));
            //chords.Add(new Chord(new Byte[] { 76 }, 4, 1));
            //chords.Add(new Chord(new Byte[] { 76 }, 4, 1));

            BinaryReader br;
            try
            {
                br = new BinaryReader(new FileStream(fileName, FileMode.Open));
            } catch (IOException e)
            {
                throw e;
            }

            //throw new IOException("Not sure how to read midi files yet");

            br.Close();
        }

        public byte[] ToBytes()
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
