/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace ID
{
    juce::String getCellId(int index)
    {
        return "CELL_" + juce::String{ index };
    }
}

//==============================================================================
SoundOfLifeAudioProcessor::SoundOfLifeAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    apvts(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    presetManager = std::make_unique<jr::PresetManager>(apvts);

    addListenersToApvts();

    lifeGridService.addListener(this);
    
    startTimer(timerIntervalMs);
}

SoundOfLifeAudioProcessor::~SoundOfLifeAudioProcessor()
{
    removeListenersFromApvts();
    lifeGridService.removeListener(this);
}

//==============================================================================
const juce::String SoundOfLifeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SoundOfLifeAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SoundOfLifeAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SoundOfLifeAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SoundOfLifeAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SoundOfLifeAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SoundOfLifeAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SoundOfLifeAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SoundOfLifeAudioProcessor::getProgramName (int index)
{
    return {};
}

void SoundOfLifeAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SoundOfLifeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void SoundOfLifeAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SoundOfLifeAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SoundOfLifeAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool SoundOfLifeAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SoundOfLifeAudioProcessor::createEditor()
{
    return new SoundOfLifeAudioProcessorEditor (*this);
}

//==============================================================================
void SoundOfLifeAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void SoundOfLifeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName(apvts.state.getType()))
        {
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SoundOfLifeAudioProcessor();
}

//==============================================================================
void SoundOfLifeAudioProcessor::timerCallback()
{
    if (timerOn)
    {
        lifeGridService.nextGeneration();
        for (auto listener : timerListeners)
        {
            listener->onTimerBeat();
        }
    }
}

void SoundOfLifeAudioProcessor::setTimerInterval(int timeInMs)
{
    timerIntervalMs = timeInMs;
    startTimer(timerIntervalMs);
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout SoundOfLifeAudioProcessor::createParameterLayout()
{
    int numCells = lifeGridService.numRows * lifeGridService.rowSize;

    juce::AudioProcessorValueTreeState::ParameterLayout layout{};

    for (int i{}; i < numCells; i++)
    {
        const juce::String cellId = ID::getCellId(i);
        layout.add(std::make_unique<juce::AudioParameterBool>(cellId, cellId, false));
    }
    
    return layout;
}

std::function<void(bool)> SoundOfLifeAudioProcessor::getListenerCallbackForCell(int cellIndex)
{
    int m = static_cast<int> (cellIndex / lifeGridService.getRowSize());
    int n = cellIndex % lifeGridService.getRowSize();

    return [=](bool newValue)
        {
            lifeGridService.getCell(m, n)->setNextValue(newValue);
            lifeGridService.getCell(m, n)->triggerGeneration();
        };
}

void SoundOfLifeAudioProcessor::addListenersToApvts()
{
    int numCells = lifeGridService.numRows * lifeGridService.rowSize;

    for (int i{}; i < numCells; i++)
    {
        paramListeners.push_back(std::make_unique<jr::ApvtsListener>(getListenerCallbackForCell(i)));
        apvts.addParameterListener(ID::getCellId(i), paramListeners.at(i).get());
    }
}

void SoundOfLifeAudioProcessor::removeListenersFromApvts()
{
    int numCells = lifeGridService.numRows * lifeGridService.rowSize;

    for (int i{}; i < numCells; i++)
    {
        apvts.removeParameterListener(ID::getCellId(i), paramListeners.at(i).get());
    }
}

//==============================================================================
void SoundOfLifeAudioProcessor::updateCellParam(std::vector<int> const& cellIndexes)
{
    for (int const cellIndex : cellIndexes)
    {
        auto param = apvts.getParameter(ID::getCellId(cellIndex));
        bool isAlive = param->getValue() > 0.1f;
        param->setValueNotifyingHost(isAlive ? 0.0f : 1.0f); // flips the alive state so that the cells value changes
    }
}

//==============================================================================
void SoundOfLifeAudioProcessor::removeTimerListener(jr::TimerListener* l)
{
    auto iter = std::remove(timerListeners.begin(), timerListeners.end(), l);
    timerListeners.erase(iter, timerListeners.end());
}
