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
#include "Service/jr_TimerListener.h"
#include "LookAndFeel/jr_StyleSheet.h"
#include "GUI/ToggleAttachment.h"

//==============================================================================
/**
*/
class SoundOfLifeAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener, public jr::TimerListener
{
public:
    SoundOfLifeAudioProcessorEditor (SoundOfLifeAudioProcessor&);
    ~SoundOfLifeAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(juce::Slider* slider) override;

    void onTimerBeat() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SoundOfLifeAudioProcessor& audioProcessor;

    jr::CustomLookAndFeel customLookAndFeel;

    jr::PresetPanel presetPanel;

    std::shared_ptr<jr::LifeGridGUI> lifeGrid;
    std::unique_ptr<jr::LifeGridAttachment> gridAttachment;

    juce::TextButton nextButton{ "Next Generation" }, randomButton{ "Randomise" };
    juce::ToggleButton timerButton{ "Auto On/Off" }, tempoSyncButton{ "Tempo Sync On/Off" }, acceptNoteOffButton{ "Accept MIDI Note Off Input" };

    juce::Slider frequencySlider{ juce::Slider::SliderStyle::Rotary, juce::Slider::TextBoxBelow };
    juce::Slider velocitySlider{ juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::NoTextBox };
    juce::Label frequencyLabel, velocityLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackSliderAttachment;

    jr::Blinker blinker;

    std::unique_ptr<jr::ToggleButtonAttachment> timerToggleAttachment, tempoSyncToggleAttachment, acceptNoteOffToggleAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundOfLifeAudioProcessorEditor)
};
