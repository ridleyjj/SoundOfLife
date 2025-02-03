/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI/LifeGridGUI.h"
#include "GUI/LifeGridAttachment.h"
#include "GUI/jr_PresetPanel.h"
#include "GUI/Blinker.h"

//==============================================================================
/**
*/
class SoundOfLifeAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener
{
public:
    SoundOfLifeAudioProcessorEditor (SoundOfLifeAudioProcessor&);
    ~SoundOfLifeAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(juce::Slider* slider) override;

    void toggleBlinker();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SoundOfLifeAudioProcessor& audioProcessor;

    jr::PresetPanel presetPanel;

    std::shared_ptr<jr::LifeGridGUI> lifeGrid;
    std::unique_ptr<jr::LifeGridAttachment> gridAttachment;

    juce::TextButton nextButton{ "Next Generation" };
    juce::TextButton randomButton{ "Randomise" };
    juce::ToggleButton timerButton{ "Timer On/Off" };

    juce::Slider frequencySlider{ juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextBoxBelow };
    juce::Label frequencyLabel;

    jr::Blinker blinker;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundOfLifeAudioProcessorEditor)
};
