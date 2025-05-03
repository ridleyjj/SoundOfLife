/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Utils/jr_JuceUtils.h"
#include "Service/id_constants.h"

//==============================================================================
SoundOfLifeAudioProcessorEditor::SoundOfLifeAudioProcessorEditor (SoundOfLifeAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), presetPanel(p.getPresetManager())
{
    setLookAndFeel(&customLookAndFeel);
    juce::LookAndFeel::setDefaultLookAndFeel(&customLookAndFeel);

    lifeGrid = std::make_shared<jr::LifeGridGUI>(p.getLifeGridService());
    gridAttachment = std::make_unique<jr::LifeGridAttachment>(*lifeGrid, p.getAPVTS());

    addAndMakeVisible(*lifeGrid.get());
    addAndMakeVisible(nextButton);
    addAndMakeVisible(timerButton);
    addAndMakeVisible(tempoSyncButton);
    addAndMakeVisible(randomButton);
    addAndMakeVisible(blinker);
    addAndMakeVisible(acceptNoteOffButton);

    jr::JuceUtils::initSimpleSliderWithRange(this, &frequencySlider, &frequencyLabel, "Interval (ms)", 250, 2000, 1, true);
    frequencySlider.setValue(p.getTimerIntervalMs(), juce::dontSendNotification);
    frequencySlider.addListener(this);

    timerToggleAttachment = std::make_unique<jr::ToggleButtonAttachment>(timerButton, ID::AUTO_GEN_MODE, p.getAPVTS());
    tempoSyncToggleAttachment = std::make_unique<jr::ToggleButtonAttachment>(tempoSyncButton, ID::TEMPO_SYNC_MODE, p.getAPVTS());
    acceptNoteOffToggleAttachment = std::make_unique<jr::ToggleButtonAttachment>(acceptNoteOffButton, ID::ACCEPT_MIDI_NOTE_OFF_INPUT, p.getAPVTS());

    nextButton.onClick = [&]() { audioProcessor.getLifeGridService().nextGeneration(); };
    randomButton.onClick = [&]() { audioProcessor.getLifeGridService().randomiseSetup(); };

    nextButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    randomButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);

    addAndMakeVisible(presetPanel);

    p.addTimerListener(this);

    setSize (400, 650);
}

SoundOfLifeAudioProcessorEditor::~SoundOfLifeAudioProcessorEditor()
{
    audioProcessor.removeTimerListener(this);
    juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
    setLookAndFeel(nullptr);
}

//==============================================================================
void SoundOfLifeAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colour::fromRGB(96, 113, 150));
}

void SoundOfLifeAudioProcessorEditor::resized()
{
    auto contentContainer = getBounds();
    auto presetRow = contentContainer.removeFromTop(50);
    auto topRow = contentContainer.removeFromTop(200);

    presetPanel.setBounds(presetRow);

    auto firstButtonSection = topRow.removeFromTop(50).reduced(5, 5);
    nextButton.setBounds(firstButtonSection.removeFromLeft(contentContainer.proportionOfWidth(0.33f)).reduced(4));
    timerButton.setBounds(firstButtonSection.removeFromLeft(contentContainer.proportionOfWidth(0.33f)).reduced(4));
    randomButton.setBounds(firstButtonSection.reduced(4));
    
    frequencySlider.setBounds(topRow.removeFromLeft(contentContainer.proportionOfWidth(0.34f)).reduced(4, 24));

    acceptNoteOffButton.setBounds(topRow.removeFromTop(topRow.proportionOfHeight(0.5f)).reduced(2));
    tempoSyncButton.setBounds(topRow.removeFromLeft(contentContainer.proportionOfWidth(0.43f)).reduced(4));
    
    blinker.setBounds(topRow.reduced(4));

    lifeGrid->setBounds(contentContainer);
}

//============================= Callbacks ================================

void SoundOfLifeAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &frequencySlider)
    {
        audioProcessor.setTimerInterval(frequencySlider.getValue());
    }
}

void SoundOfLifeAudioProcessorEditor::onTimerBeat()
{
    blinker.startBlink(audioProcessor.getTimerIntervalMs());
}