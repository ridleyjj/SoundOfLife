#pragma once

#include <cmath>
#include <array>
#include <JuceHeader.h>

namespace jr
{
    namespace ScaleConstants
    {
        const std::array<int, 7> majIntervals = { 0, 2, 4, 5, 7, 9, 11 };
        const std::array<int, 7> minIntervals = { 0, 2, 3, 5, 7, 8, 10 };

        const juce::StringArray scaleTypes{ "Chromatic", "Major", "Minor" };
        const juce::StringArray baseNotes{ "C", "C#/Db", "D", "D#/Eb", "E", "F", "F#/Gb", "G", "G#/Ab", "A", "A#/Bb", "B" };
    }
}