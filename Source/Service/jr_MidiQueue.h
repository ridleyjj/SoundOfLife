#pragma once

#include <JuceHeader.h>

namespace jr
{
    /*
    A class for queueing MIDI from a non-audio thread, to be consumed in the audio thread, all in a thread-safe way
    */
    class MidiQueue
    {
    public:
        void push(const juce::MidiMessage& m)
        {
            const juce::ScopedLock sl(lock);
            queue.add({ m, 0 }); // samplePosition is 0 so notes play asap
        }

        void popAllTo(juce::MidiBuffer& target)
        {
            const juce::ScopedLock sl(lock);
            for (auto& msg : queue)
                target.addEvent(msg.message, msg.samplePosition);
            queue.clear();
        }

    private:
        juce::CriticalSection lock;
        struct TimedMidiMessage
        {
            juce::MidiMessage message;
            int samplePosition;
        };
        juce::Array<TimedMidiMessage> queue;
    };
};