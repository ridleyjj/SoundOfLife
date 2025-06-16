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
    addAndMakeVisible(clearButton);

    jr::JuceUtils::initSimpleSliderWithRange(this, &frequencySlider, &frequencyLabel, "Interval (ms)", 250, 2000, 1, true);
    frequencySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.getAPVTS(), ID::FREQUENCY, frequencySlider);
    
    jr::JuceUtils::initSimpleSlider(this, &velocitySlider, &velocityLabel, "Velocity");
    attackSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.getAPVTS(), ID::VELOCITY, velocitySlider);

    timerToggleAttachment = std::make_unique<jr::ToggleButtonAttachment>(timerButton, ID::AUTO_GEN_MODE, p.getAPVTS());
    tempoSyncToggleAttachment = std::make_unique<jr::ToggleButtonAttachment>(tempoSyncButton, ID::TEMPO_SYNC_MODE, p.getAPVTS());
    acceptNoteOffToggleAttachment = std::make_unique<jr::ToggleButtonAttachment>(acceptNoteOffButton, ID::ACCEPT_MIDI_NOTE_OFF_INPUT, p.getAPVTS());

    nextButton.onClick = [&]() { audioProcessor.getLifeGridService().nextGeneration(); };
    randomButton.onClick = [&]() { audioProcessor.getLifeGridService().randomiseSetup(); };
    clearButton.onClick = [&]() { audioProcessor.getLifeGridService().clearGrid(); };

    nextButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    randomButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);

    addAndMakeVisible(presetPanel);
    addAndMakeVisible(scalePanel);
    addAndMakeVisible(sectionALabel);
    addAndMakeVisible(sectionBLabel);

    p.addTimerListener(this);

    setSize (412, 607);
    setResizable(false, false);
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
    g.setColour(juce::Colours::black);
    g.drawRect(sectionABox, 2.0f);
    g.drawRect(sectionBBox, 2.0f);
    g.drawRect(sectionCBox, 2.0f);
}

void SoundOfLifeAudioProcessorEditor::resized()
{
    auto contentContainer = getBounds();
    auto presetRow = contentContainer.removeFromTop(64);
    auto sectionA = contentContainer.removeFromTop(139);
    auto sectionB = sectionA.removeFromRight(sectionA.proportionOfWidth(0.5f));

    auto sectionLabelSize = sectionA.proportionOfHeight(0.15f);
    auto sectionRowSize = sectionA.proportionOfHeight(0.25f);

    auto sectionC = contentContainer.removeFromTop(404);

    presetPanel.setBounds(presetRow);

    // section A
    sectionABox = sectionA.reduced(1);
    sectionA.removeFromTop(3); // top margin for label
    sectionALabel.setBounds(sectionA.removeFromTop(sectionLabelSize).reduced(3));
    scalePanel.setBounds(sectionA.removeFromTop(sectionRowSize * 2.0f).reduced(3, 0));
    velocitySlider.setBounds(sectionA.reduced(3, 10));
    jr::JuceUtils::drawSliderLabelWithCustomGap(velocitySlider, velocityLabel, 18);

    // section B
    sectionBBox = sectionB.reduced(1);
    sectionB.removeFromTop(3); // top margin for label
    sectionBLabel.setBounds(sectionB.removeFromTop(sectionLabelSize).reduced(3));
    timerButton.setBounds(sectionB.removeFromTop(sectionRowSize * 0.8f).reduced(3, 6));
    tempoSyncButton.setBounds(sectionB.removeFromTop(sectionRowSize * 0.8f).reduced(3, 6));
    sectionB.removeFromTop(sectionRowSize * 0.4f); // spacer between checkboxes and slider
    frequencySlider.setBounds(sectionB.reduced(3, 10));
    jr::JuceUtils::drawSliderLabelWithCustomGap(frequencySlider, frequencyLabel, 18);

    sectionCBox = sectionC.reduced(1);
    sectionC = sectionC.reduced(1);
    sectionC.removeFromTop(2); // margin above button row
    auto buttonRow = sectionC.removeFromTop(41).reduced(22, 3);
    nextButton.setBounds(buttonRow.removeFromLeft(172).reduced(6, 0));
    randomButton.setBounds(buttonRow.removeFromLeft(127).reduced(6, 0));
    clearButton.setBounds(buttonRow.reduced(6, 0));
    sectionC.removeFromBottom(sectionC.proportionOfHeight(0.02f));
    lifeGrid->setBounds(sectionC.removeFromTop(352));
}

//============================= Callbacks ================================

void SoundOfLifeAudioProcessorEditor::onTimerBeat()
{
    blinker.startBlink(audioProcessor.getTimerIntervalMs());
}