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
class SoundOfLifeAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    SoundOfLifeAudioProcessorEditor (SoundOfLifeAudioProcessor&);
    ~SoundOfLifeAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SoundOfLifeAudioProcessor& audioProcessor;

    jr::LifeGrid lifeGrid;

    juce::TextButton nextButton{ "Next Generation" };
    juce::TextButton randomButton{ "Randomise" };
    juce::ToggleButton timerButton{ "Timer On/Off" };

    bool timerOn{ false };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundOfLifeAudioProcessorEditor)
};
