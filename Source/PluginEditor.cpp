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
    : AudioProcessorEditor (&p), audioProcessor (p), presetPanel(p.getPresetManager()), scalePanel(p.getScaleManager())
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
    frequencySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.getAPVTS(), ID::FREQUENCY, frequencySlider);
    
    jr::JuceUtils::initSimpleSlider(this, &velocitySlider, &velocityLabel, "Velocity");
    attackSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.getAPVTS(), ID::VELOCITY, velocitySlider);

    timerToggleAttachment = std::make_unique<jr::ToggleButtonAttachment>(timerButton, ID::AUTO_GEN_MODE, p.getAPVTS());
    tempoSyncToggleAttachment = std::make_unique<jr::ToggleButtonAttachment>(tempoSyncButton, ID::TEMPO_SYNC_MODE, p.getAPVTS());
    acceptNoteOffToggleAttachment = std::make_unique<jr::ToggleButtonAttachment>(acceptNoteOffButton, ID::ACCEPT_MIDI_NOTE_OFF_INPUT, p.getAPVTS());

    nextButton.onClick = [&]() { audioProcessor.getLifeGridService().nextGeneration(); };
    randomButton.onClick = [&]() { audioProcessor.getLifeGridService().randomiseSetup(); };

    nextButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    randomButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);

    addAndMakeVisible(presetPanel);
    addAndMakeVisible(scalePanel);

    p.addTimerListener(this);

    setSize (600, 650);
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
    g.fillAll(juce::Colours::white);
}

void SoundOfLifeAudioProcessorEditor::resized()
{
    auto contentContainer = getBounds();
    auto presetRow = contentContainer.removeFromTop(50);
    auto sectionA = contentContainer.removeFromTop(200);
    auto sectionB = sectionA.removeFromRight(sectionA.proportionOfWidth(0.5f));
    auto sectionRowSize = sectionA.proportionOfHeight(0.33f);
    int sideSectionSize = 100;
    auto sectionC = contentContainer.removeFromLeft(sideSectionSize);
    auto sectionD = contentContainer.removeFromRight(sideSectionSize);

    presetPanel.setBounds(presetRow);

    // section A
    scalePanel.setBounds(sectionA.removeFromTop(sectionRowSize));
    acceptNoteOffButton.setBounds(sectionA.removeFromTop(sectionRowSize).reduced(8));

    // section B
    auto firstRowSectionB = sectionB.removeFromTop(sectionRowSize);
    timerButton.setBounds(firstRowSectionB.removeFromLeft(firstRowSectionB.proportionOfWidth(0.5f)).reduced(4));
    tempoSyncButton.setBounds(firstRowSectionB.reduced(4));
    
    frequencySlider.setBounds(sectionB.removeFromTop(sectionRowSize).reduced(4, 16));

    velocitySlider.setBounds(sectionB.removeFromTop(sectionRowSize).reduced(4, 16));

    // section C
    randomButton.setBounds(sectionC.reduced(4, 179));

    // section D
    sectionD.removeFromTop(150);
    sectionD.removeFromBottom(150);
    nextButton.setBounds(sectionD.removeFromTop(sectionD.proportionOfHeight(0.5f)).reduced(4));
    blinker.setBounds(sectionD.reduced(4));

    lifeGrid->setBounds(contentContainer);
}

//============================= Callbacks ================================

void SoundOfLifeAudioProcessorEditor::onTimerBeat()
{
    blinker.startBlink(audioProcessor.getTimerIntervalMs());
}