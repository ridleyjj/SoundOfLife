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

    setSize (400, 400);
}

SoundOfLifeAudioProcessorEditor::~SoundOfLifeAudioProcessorEditor()
{
}

//==============================================================================
void SoundOfLifeAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::whitesmoke);

    
}

void SoundOfLifeAudioProcessorEditor::resized()
{
    lifeGrid.setBounds(getBounds().reduced(10));
}
