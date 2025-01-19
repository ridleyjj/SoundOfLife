/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SoundOfLifeAudioProcessorEditor::SoundOfLifeAudioProcessorEditor (SoundOfLifeAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(lifeGrid);
    addAndMakeVisible(nextButton);
    addAndMakeVisible(timerButton);
    addAndMakeVisible(randomButton);

    timerButton.onClick = [&]() { timerOn = !timerOn; };
    nextButton.onClick = [&]() { lifeGrid.nextGeneration(); };
    randomButton.onClick = [&]() { lifeGrid.randomiseSetup(); };

    setSize (400, 500);

    startTimer(1000);
}

SoundOfLifeAudioProcessorEditor::~SoundOfLifeAudioProcessorEditor()
{
}

//==============================================================================
void SoundOfLifeAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::darkslategrey);

    
}

void SoundOfLifeAudioProcessorEditor::resized()
{
    auto contentContainer = getBounds().reduced(10);
    auto topRow = contentContainer.removeFromTop(contentContainer.proportionOfHeight(0.2f));
    nextButton.setBounds(topRow.removeFromLeft(contentContainer.proportionOfWidth(0.33f)).reduced(15, 5));
    randomButton.setBounds(topRow.removeFromLeft(contentContainer.proportionOfWidth(0.33f)).reduced(15, 5));
    timerButton.setBounds(topRow.reduced(15, 5));
    lifeGrid.setBounds(contentContainer);
}

void SoundOfLifeAudioProcessorEditor::timerCallback()
{
    if (timerOn)
    {
        lifeGrid.nextGeneration();
    }
}