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

    setSize (550, 810);
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
    auto presetRow = contentContainer.removeFromTop(85);
    auto sectionA = contentContainer.removeFromTop(185);
    auto sectionB = sectionA.removeFromRight(sectionA.proportionOfWidth(0.5f));

    auto sectionLabelSize = sectionA.proportionOfHeight(0.15f);
    auto sectionRowSize = sectionA.proportionOfHeight(0.25f);

    auto sectionC = contentContainer;

    presetPanel.setBounds(presetRow);

    // section A
    sectionABox = sectionA.reduced(1);
    sectionA.removeFromTop(4); // top margin for label
    sectionALabel.setBounds(sectionA.removeFromTop(sectionLabelSize).reduced(4));
    scalePanel.setBounds(sectionA.removeFromTop(sectionRowSize * 2.0f).reduced(4, 0));
    velocitySlider.setBounds(sectionA.reduced(4, 12));

    // section B
    sectionBBox = sectionB.reduced(1);
    sectionB.removeFromTop(4); // top margin for label
    sectionBLabel.setBounds(sectionB.removeFromTop(sectionLabelSize).reduced(4));
    timerButton.setBounds(sectionB.removeFromTop(sectionRowSize * 0.8f).reduced(4, 8));
    tempoSyncButton.setBounds(sectionB.removeFromTop(sectionRowSize * 0.8f).reduced(4, 8));
    sectionB.removeFromTop(sectionRowSize * 0.4f); // spacer between checkboxes and slider
    frequencySlider.setBounds(sectionB.reduced(4, 12));

    sectionCBox = sectionC.reduced(1);
    sectionC = sectionC.reduced(1);
    auto buttonRow = sectionC.removeFromTop(54).reduced(19, 6);
    nextButton.setBounds(buttonRow.removeFromLeft(230).reduced(15, 0));
    randomButton.setBounds(buttonRow.removeFromLeft(170).reduced(15, 0));
    clearButton.setBounds(buttonRow.reduced(15, 0));
    sectionC.removeFromBottom(4);
    lifeGrid->setBounds(sectionC);
}

//============================= Callbacks ================================

void SoundOfLifeAudioProcessorEditor::onTimerBeat()
{
    blinker.startBlink(audioProcessor.getTimerIntervalMs());
}