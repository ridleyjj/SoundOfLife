#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "Resources/FontResources.h"

namespace jr
{
    // fonts
    namespace StyleSheet
    {

        static juce::Typeface::Ptr boldFont  { juce::Typeface::createSystemTypefaceFor (Resources::WorkSansSemiBold_ttf, Resources::WorkSansSemiBold_ttfSize) };
        static juce::Typeface::Ptr plainFont { juce::Typeface::createSystemTypefaceFor (Resources::WorkSansRegular_ttf, Resources::WorkSansRegular_ttfSize) };

    }

    class CustomLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        CustomLookAndFeel()
        {
            setDefaultSansSerifTypeface(StyleSheet::boldFont);
        }


    private:
    };
}
