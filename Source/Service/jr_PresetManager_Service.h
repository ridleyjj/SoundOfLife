#pragma once

#include <JuceHeader.h>

namespace jr
{
    class PresetManager : juce::ValueTree::Listener
    {
    public:
        static const juce::File defaultDirectory;
        static const juce::String extension;
        static const juce::String underscore;
        static const juce::String presetNameProperty;
        static const std::vector<juce::String> excludedParams;

        PresetManager(juce::AudioProcessorValueTreeState &_apvts);

        void savePreset(const juce::String &presetName);
        void deletePreset(const juce::String &presetName);
        void loadPreset(const juce::String &presetName);
        juce::StringArray getAllPresets();

        juce::String getCurrentPreset() { return currentPreset.toString(); }

        bool getIsCurrentPresetUserPreset() { return currentIsUserPreset; }

    private:
        void valueTreeRedirected(juce::ValueTree &treeWhichHasBeenChanged) override;

        juce::File getUserPresetFile(const juce::String &presetName);
        const char* getBinaryPresetFile(const juce::String &presetName);

        void updateIsUserPreset();

        /*
        Takes in a value tree that is about to be used to replace the apvts state, and first copies over the values
        of any Excluded Params from the current apvts to the new value tree (thereby retaining their current values and
        excluding them from the preset system)
        */
        void overrideExcludedParams(juce::ValueTree& valueTree);

        juce::AudioProcessorValueTreeState &apvts;
        juce::Value currentPreset;
        bool currentIsUserPreset{ false };
    };
}
