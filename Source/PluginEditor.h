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
#include "GUI/jr_ScalePanel.h"

//==============================================================================
/**
*/
class SoundOfLifeAudioProcessorEditor  : public juce::AudioProcessorEditor, public jr::TimerListener
{
public:
    SoundOfLifeAudioProcessorEditor (SoundOfLifeAudioProcessor&);
    ~SoundOfLifeAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void onTimerBeat() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SoundOfLifeAudioProcessor& audioProcessor;

    jr::CustomLookAndFeel customLookAndFeel;

    jr::PresetPanel presetPanel;

    jr::ScalePanel scalePanel;

    std::shared_ptr<jr::LifeGridGUI> lifeGrid;
    std::unique_ptr<jr::LifeGridAttachment> gridAttachment;

    juce::TextButton nextButton{ "Next Generation" }, randomButton{ "Randomise" }, clearButton{ "Clear" };
    juce::ToggleButton timerButton{ "Auto On/Off" }, tempoSyncButton{ "Tempo Sync On/Off" }, acceptNoteOffButton{ "Accept MIDI Note Off Input" };

    juce::Slider frequencySlider{ juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextBoxBelow };
    juce::Slider velocitySlider{ juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextBoxBelow };
    juce::Label frequencyLabel, velocityLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackSliderAttachment, frequencySliderAttachment;

    jr::Blinker blinker;

    juce::Rectangle<int> sectionABox, sectionBBox, sectionCBox;

    juce::Label sectionALabel{ "sectionALabel", "MIDI Note Output" }, sectionBLabel{ "sectionBLabel", "Timing" };

    std::unique_ptr<jr::ToggleButtonAttachment> timerToggleAttachment, tempoSyncToggleAttachment, acceptNoteOffToggleAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundOfLifeAudioProcessorEditor)
};
