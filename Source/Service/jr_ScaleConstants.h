#pragma once

#include <cmath>
#include <array>

namespace jr
{
    namespace ScaleConstants
    {
        const std::array<int, 7> majIntervals = { 0, 2, 4, 5, 7, 9, 11 };
        const std::array<int, 7> minIntervals = { 0, 2, 3, 5, 7, 8, 10 };
        const std::array<int, 5> majPentIntervals = { 0, 2, 4, 7, 9 };
        const std::array<int, 5> minPentIntervals = { 0, 3, 5, 7, 10 };

        template<size_t N>
        int getNoteNumberForScale(const std::array<int, N> scale, int scaleIndex, int baseNote = 0)
        {
            int octave = floor(scaleIndex / N);
            octave = octave % 10; // wrap octaves to stay in MIDI range
            scaleIndex = scaleIndex % (N - 1);
            int noteNumber = baseNote + (octave * 12) + scale[scaleIndex];

            // only return valid MIDI
            while (noteNumber > 127)
            {
                noteNumber -= 12;
            }

            return noteNumber;
        }

        int getMajInterval(int index, int baseNote = 0)
        {
            return getNoteNumberForScale(majIntervals, index);
        }

        int getMinInterval(int index, int baseNote = 0)
        {
            return getNoteNumberForScale(minIntervals, index);
        }

        int getMajPentInterval(int index, int baseNote = 0)
        {
            return getNoteNumberForScale(majPentIntervals, index);
        }

        int getMinPentInterval(int index, int baseNote = 0)
        {
            return getNoteNumberForScale(minPentIntervals, index);
        }
    }
}