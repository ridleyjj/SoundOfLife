/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

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

    startTimer(getTimerIntervalMs());
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

bool SoundOfLifeAudioProcessor::isNewBeat()
{
    bool isBeat = false;

    auto playHead = getPlayHead();

    if (playHead != nullptr)
    {
        auto positionInfo = playHead->getPosition();
        if (positionInfo.hasValue())
        {
            auto positionInQuarterNotes = positionInfo->getPpqPosition();
            if (positionInQuarterNotes.hasValue())
            {
                auto newNote = floor(*positionInQuarterNotes);
                if (newNote != currentNote)
                {
                    isBeat = true;
                }

                currentNote = newNote;
            }
        }
    }
    return isBeat;
}

void SoundOfLifeAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    processIncomingMidiMessages(midiMessages);

    buffer.clear();
    midiMessages.clear();

    if (isAutoModeOn() && isTempoSyncModeOn() && isNewBeat())
        lifeGridService.nextGeneration();

    midiQueue.popAllTo(midiMessages);
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
    if (isAutoModeOn() && !isTempoSyncModeOn())
    {
        lifeGridService.nextGeneration();
        for (auto listener : timerListeners)
        {
            listener->onTimerBeat();
        }
    }
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

    layout.add(std::make_unique<juce::AudioParameterBool>(ID::AUTO_GEN_MODE, ID::AUTO_GEN_MODE, false));
    layout.add(std::make_unique<juce::AudioParameterBool>(ID::TEMPO_SYNC_MODE, ID::TEMPO_SYNC_MODE, false));
    layout.add(std::make_unique<juce::AudioParameterBool>(ID::ACCEPT_MIDI_NOTE_OFF_INPUT, ID::ACCEPT_MIDI_NOTE_OFF_INPUT, true));
    layout.add(std::make_unique<juce::AudioParameterFloat>(ID::VELOCITY, ID::VELOCITY, 0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterInt>(ID::FREQUENCY, ID::FREQUENCY, 250, 2000, 1000));
    
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
            addMidiMessageFromCell(cellIndex, newValue);
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

    apvts.addParameterListener(ID::FREQUENCY, &frequencyListener);
}

void SoundOfLifeAudioProcessor::removeListenersFromApvts()
{
    int numCells = lifeGridService.numRows * lifeGridService.rowSize;

    for (int i{}; i < numCells; i++)
    {
        apvts.removeParameterListener(ID::getCellId(i), paramListeners.at(i).get());
    }

    apvts.removeParameterListener(ID::FREQUENCY, &frequencyListener);
}

//==============================================================================
void SoundOfLifeAudioProcessor::updateCellParam(std::set<int> const& cellIndexes)
{
    for (int const cellIndex : cellIndexes)
    {
        auto param = apvts.getParameter(ID::getCellId(cellIndex));
        bool isAlive = param->getValue() > 0.1f;
        param->setValueNotifyingHost(isAlive ? 0.0f : 1.0f); // flips the alive state so that the cells value changes
    }
}

void SoundOfLifeAudioProcessor::updateSingleCellParamWithValue(int const index, bool const isAlive)
{
    auto param = apvts.getParameter(ID::getCellId(index));
    param->setValueNotifyingHost(isAlive);
}

//==============================================================================
int SoundOfLifeAudioProcessor::getMidiNoteFromCellIndex(int cellIndex)
{
}

void SoundOfLifeAudioProcessor::addMidiMessageFromCell(int cellIndex, bool isAlive)
{
    juce::MidiMessage m = isAlive ? getNoteOnFromCell(cellIndex) : getNoteOffFromCell(cellIndex);
    midiQueue.push(m);
}

juce::MidiMessage SoundOfLifeAudioProcessor::getNoteOnFromCell(int cellIndex)
{
    float velocity = 0.1f + 0.7f * apvts.getParameter(ID::VELOCITY)->getValue();
    return juce::MidiMessage::noteOn(1, getMidiNoteFromCellIndex(cellIndex), velocity);
}

juce::MidiMessage SoundOfLifeAudioProcessor::getNoteOffFromCell(int cellIndex)
{
    return juce::MidiMessage::noteOff(1, getMidiNoteFromCellIndex(cellIndex), 1.0f);
}

void SoundOfLifeAudioProcessor::processIncomingMidiMessages(juce::MidiBuffer& midiMessages)
{
    if (!midiMessages.isEmpty())
    {
        for (const juce::MidiMessageMetadata metadata : midiMessages)
        {
            auto message = metadata.getMessage();
            if (message.isNoteOn() || (apvts.getParameter(ID::ACCEPT_MIDI_NOTE_OFF_INPUT)->getValue() && message.isNoteOff()))
            {
                int numCells = lifeGridService.numRows * lifeGridService.rowSize;
                if (message.getNoteNumber() >= 23 && message.getNoteNumber() < getMidiNoteFromCellIndex(numCells))
                {
                    updateSingleCellParamWithValue(getCellIndexFromMidiNote(message.getNoteNumber()), message.isNoteOn());
                }
            }
        }
    }
}

//==============================================================================
void SoundOfLifeAudioProcessor::removeTimerListener(jr::TimerListener* l)
{
    auto iter = std::remove(timerListeners.begin(), timerListeners.end(), l);
    timerListeners.erase(iter, timerListeners.end());
}
