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
#include "Service/jr_TimerListener.h";
#include "Service/id_constants.h"

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
    bool getIsTimerOn() { return timerOn; }

    //==============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }

    //==============================================================================
    void updateCellParam(std::vector<int> const& cellIndexes) override;

    //==============================================================================
    void addTimerListener(jr::TimerListener* l) { timerListeners.push_back(l); }
    void removeTimerListener(jr::TimerListener* l);

    /*
    Returns true if the current sample block crosses over to a new beat in the project's tempo. Only call from processBlock()
    */
    bool isNewBeat();

    bool isAutoModeOn() { return (bool)*apvts.getRawParameterValue(ID::AUTO_GEN_MODE); }
    bool isTempoSyncModeOn() { return (bool)*apvts.getRawParameterValue(ID::TEMPO_SYNC_MODE); }

private:
    //=================== methods =======================
    void addMidiMessageFromCell(int cellIndex, bool isAlive);
    int getMidiNoteFromCellIndex(int cellIndex) { return cellIndex + 23; }
    juce::MidiMessage getNoteOnFromCell(int cellIndex);
    juce::MidiMessage getNoteOffFromCell(int cellIndex);

    //=================== params ========================
    std::unique_ptr<jr::PresetManager> presetManager;

    jr::LifeGridService lifeGridService;

    bool timerOn{ false };
    int timerIntervalMs{ 1000 };

    juce::AudioProcessorValueTreeState apvts;
    std::vector<std::unique_ptr<jr::ApvtsListener>> paramListeners{};

    void addListenersToApvts();
    void removeListenersFromApvts();
    std::function<void(bool)> getListenerCallbackForCell(int cellIndex);

    std::vector<jr::TimerListener*> timerListeners{};

    juce::MidiBuffer midiOutBuffer{};

    double currentNote{};

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundOfLifeAudioProcessor)
};
