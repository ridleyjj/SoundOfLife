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

        juce::Font getTextButtonFont(juce::TextButton&, int buttonHeight) override
        {
            return withDefaultMetrics(juce::FontOptions{ juce::jmin(20.0f, (float)buttonHeight * 0.8f) });
        }

        juce::Font getComboBoxFont(juce::ComboBox& box) override
        {
            return withDefaultMetrics(juce::FontOptions{ juce::jmin(20.0f, (float)box.getHeight() * 0.85f) });
        }

        void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
            bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
        {
            auto fontSize = juce::jmin(20.0f, (float)button.getHeight() * 0.75f);
            auto tickWidth = fontSize * 0.9f;

            drawTickBox(g, button, 4.0f, ((float)button.getHeight() - tickWidth) * 0.5f,
                tickWidth, tickWidth,
                button.getToggleState(),
                button.isEnabled(),
                shouldDrawButtonAsHighlighted,
                shouldDrawButtonAsDown);

            g.setColour(button.findColour(juce::ToggleButton::textColourId));
            g.setFont(fontSize);

            if (!button.isEnabled())
                g.setOpacity(0.5f);

            g.drawFittedText(button.getButtonText(),
                button.getLocalBounds().withTrimmedLeft(juce::roundToInt(tickWidth) + 10)
                .withTrimmedRight(2),
                juce::Justification::centredLeft, 10);
        }

        juce::Label* createSliderTextBox(juce::Slider& slider) override
        {
            auto* l = LookAndFeel_V2::createSliderTextBox(slider);

            if (getCurrentColourScheme() == LookAndFeel_V4::getGreyColourScheme() && (slider.getSliderStyle() == juce::Slider::LinearBar
                || slider.getSliderStyle() == juce::Slider::LinearBarVertical))
            {
                l->setColour(juce::Label::textColourId, juce::Colours::black.withAlpha(0.7f));
            }

            l->setFont(juce::FontOptions{ juce::jmin(20.0f, (float)slider.getHeight() * 0.8f) });

            return l;
        }


    private:
    };
}
