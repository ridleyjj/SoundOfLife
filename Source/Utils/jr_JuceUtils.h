#pragma once

#include <JuceHeader.h>

namespace jr
{
    class JuceUtils
    {
    public:
        static void initSimpleSlider(juce::Component* parent, juce::Slider* slider, juce::Label* label, const juce::String& name, bool isTextBoxEditable = false)
        {
            // slider init
            slider->setTextBoxIsEditable(isTextBoxEditable);
            parent->addAndMakeVisible(slider);

            // text label init
            label->setText(name, juce::NotificationType::dontSendNotification);
            label->setJustificationType(juce::Justification::centredTop);
            label->attachToComponent(slider, false);
        }

        static void initSimpleSliderWithRange(juce::Component* parent, juce::Slider* slider, juce::Label* label, const juce::String& name, double minVal, double maxVal, double step, bool isTextBoxEditable = false)
        {
            initSimpleSlider(parent, slider, label, name, isTextBoxEditable);

            slider->setRange(minVal, maxVal, step);
        }
    };
}
