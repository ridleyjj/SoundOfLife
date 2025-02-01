/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Service/LifeGridService.h"
#include "Service/ApvtsListener.h";
#include "Service/jr_PresetManager_Service.h";

namespace ID
{
    juce::String getCellId(int index);
}

//==============================================================================
/**
*/
class SoundOfLifeAudioProcessor  : public juce::AudioProcessor, public juce::Timer, public jr::LifeGridServiceListener
{
public:
    //==============================================================================
    SoundOfLifeAudioProcessor();
    ~SoundOfLifeAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    jr::LifeGridService& getLifeGridService() { return lifeGridService; }

    //==============================================================================
    void timerCallback() override;
    void setTimerInterval(int timeInMs);
    void toggleTimer() { timerOn = !timerOn; }
    int getTimerIntervalMs() { return timerIntervalMs; }
    jr::PresetManager& getPresetManager() { return *presetManager; }

    //==============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }

    //==============================================================================
    void updateCellParam(int cellIndex) override;

private:

    std::unique_ptr<jr::PresetManager> presetManager;

    jr::LifeGridService lifeGridService;

    bool timerOn{ false };
    int timerIntervalMs{ 1000 };

    juce::AudioProcessorValueTreeState apvts;
    std::vector<std::unique_ptr<jr::ApvtsListener>> paramListeners{};

    void addListenersToApvts();
    void removeListenersFromApvts();
    std::function<void(bool)> getListenerCallbackForCell(int cellIndex);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundOfLifeAudioProcessor)
};
