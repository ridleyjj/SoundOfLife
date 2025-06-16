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
            label->setJustificationType(juce::Justification::topLeft);
            label->attachToComponent(slider, false);
        }

        static void initSimpleSliderWithRange(juce::Component* parent, juce::Slider* slider, juce::Label* label, const juce::String& name, double minVal, double maxVal, double step, bool isTextBoxEditable = false)
        {
            initSimpleSlider(parent, slider, label, name, isTextBoxEditable);

            slider->setRange(minVal, maxVal, step);
        }

        /**
        Sets the bounds of a label component relative to a slider component. Sets the label above the slider by the given gapSize
        */
        static void drawSliderLabelWithCustomGap(juce::Slider& slider, juce::Label& label, int gapSize, int labelHeight = 20)
        {
            label.setBounds(slider.getX(), (slider.getY() - gapSize), slider.getWidth(), labelHeight);
        }
    };
}
