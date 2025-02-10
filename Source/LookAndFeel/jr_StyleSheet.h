#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "Resources/BinaryData.h"

namespace jr
{
    namespace StyleSheet
    {
        // fonts
        static juce::Typeface::Ptr gdRegularFont{juce::Typeface::createSystemTypefaceFor(BinaryData::GowunDodumRegular_ttf, BinaryData::GowunDodumRegular_ttfSize)};
    }

    class CustomLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        CustomLookAndFeel()
        {
            setDefaultSansSerifTypeface(StyleSheet::gdRegularFont);
        }

    private:
    };
}
