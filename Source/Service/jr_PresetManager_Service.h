#pragma once

#include <JuceHeader.h>

namespace jr
{
    class PresetManager : juce::ValueTree::Listener
    {
    public:
        static const juce::File defaultDirectory;
        static const juce::String extension;
        static const juce::String presetNameProperty;

        PresetManager(juce::AudioProcessorValueTreeState &_apvts);

        void savePreset(const juce::String &presetName);
        void deletePreset(const juce::String &presetName);
        void loadPreset(const juce::String &presetName);
        juce::StringArray getAllPresets();

        juce::String getCurrentPreset() { return currentPreset.toString(); }
        int getCurrentPresetIndex();

    private:
        void valueTreeRedirected(juce::ValueTree &treeWhichHasBeenChanged) override;

        juce::File getPresetFile(const juce::String &presetName);

        juce::AudioProcessorValueTreeState &apvts;
        juce::Value currentPreset;
    };
}
