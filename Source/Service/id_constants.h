#pragma once
#include <juce_core/juce_core.h>

namespace ID
{
    juce::String getCellId(int index);
    const juce::String AUTO_GEN_MODE{ "Auto-Generate Mode" };
    const juce::String TEMPO_SYNC_MODE{ "Tempo-Sync Mode" };
    const juce::String ACCEPT_MIDI_NOTE_OFF_INPUT{ "Accepts MIDI Note Off Inputs" };
    const juce::String VELOCITY{ "Velocity" };
    const juce::String FREQUENCY{ "Frequency" };
}
