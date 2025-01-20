/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Utils/jr_JuceUtils.h"

//==============================================================================
SoundOfLifeAudioProcessorEditor::SoundOfLifeAudioProcessorEditor (SoundOfLifeAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(lifeGrid);
    addAndMakeVisible(nextButton);
    addAndMakeVisible(timerButton);
    addAndMakeVisible(randomButton);

    jr::JuceUtils::initSimpleSliderWithRange(this, &frequencySlider, &frequencyLabel, "Timer Frequency (ms)", 250, 2000, 1, true);
    frequencySlider.setValue(timerIntervalMs, juce::dontSendNotification);
    frequencySlider.addListener(this);

    timerButton.onClick = [&]() { timerOn = !timerOn; };
    nextButton.onClick = [&]() { lifeGrid.nextGeneration(); };
    randomButton.onClick = [&]() { lifeGrid.randomiseSetup(); };

    setSize (400, 550);

    startTimer(timerIntervalMs);
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
    auto topRow = contentContainer.removeFromTop(contentContainer.proportionOfHeight(0.27f));

    auto firstButtonSection = topRow.removeFromLeft(contentContainer.proportionOfWidth(0.33f)).reduced(15, 5);
    nextButton.setBounds(firstButtonSection.removeFromTop(topRow.proportionOfHeight(0.5f)).reduced(4));
    randomButton.setBounds(firstButtonSection.reduced(4));

    timerButton.setBounds(topRow.removeFromTop(topRow.proportionOfHeight(0.33f)).reduced(15, 5));
    frequencySlider.setBounds(topRow.reduced(4, 24));

    lifeGrid.setBounds(contentContainer);
}

//============================= Callbacks ================================

void SoundOfLifeAudioProcessorEditor::timerCallback()
{
    if (timerOn)
    {
        lifeGrid.nextGeneration();
    }
}

void SoundOfLifeAudioProcessorEditor::setTimerInterval(int timeInMs)
{
    timerIntervalMs = timeInMs;
    startTimer(timerIntervalMs);
}

void SoundOfLifeAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &frequencySlider)
    {
        setTimerInterval(frequencySlider.getValue());
    }
}