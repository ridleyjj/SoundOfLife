/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI/LifeGrid.h"

//==============================================================================
/**
*/
class SoundOfLifeAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer, public juce::Slider::Listener
{
public:
    SoundOfLifeAudioProcessorEditor (SoundOfLifeAudioProcessor&);
    ~SoundOfLifeAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;

    void setTimerInterval(int timeInMs);

    void sliderValueChanged(juce::Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SoundOfLifeAudioProcessor& audioProcessor;

    jr::LifeGrid lifeGrid;

    juce::TextButton nextButton{ "Next Generation" };
    juce::TextButton randomButton{ "Randomise" };
    juce::ToggleButton timerButton{ "Timer On/Off" };

    juce::Slider frequencySlider{ juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextBoxBelow };
    juce::Label frequencyLabel;

    bool timerOn{ false };
    int timerIntervalMs{ 1000 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundOfLifeAudioProcessorEditor)
};
