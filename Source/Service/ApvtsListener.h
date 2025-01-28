/*
  ==============================================================================

    ApvtsListener.h
    Created: 23 Nov 2024 4:30:22pm
    Author:  ridle

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace jr
{
    /*
    A simple class that inherits from juce::APVTS::Listener, with a callback that will be called on parameter changed
    */
    class ApvtsListener : public juce::AudioProcessorValueTreeState::Listener
    {
    public:
        ApvtsListener() {}

        /*
        initialises the Listener with a callback function that takes the newValue of parameter as input
        */
        ApvtsListener(std::function<void(float)> parameterChangedCallback)
        {
            setCallback(parameterChangedCallback);
        }

        void parameterChanged(const juce::String& parameterID, float newValue) override
        {
            juce::ignoreUnused(parameterID);

            if (callback)
            {
                callback(newValue);
            }
        };

        void setCallback(std::function<void(float)> parameterChangedCallback)
        {
            callback = parameterChangedCallback;
        }

    private:
        std::function<void(float)> callback;
    };

}