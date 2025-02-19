#include "jr_PresetManager_Service.h"

namespace jr
{
    const juce::File PresetManager::defaultDirectory{
        juce::File::getSpecialLocation(
            juce::File::SpecialLocationType::commonDocumentsDirectory)
            .getChildFile("RidleySound")
            .getChildFile("SoundOfLife")};
    const juce::String PresetManager::extension{"preset"};
    const juce::String PresetManager::presetNameProperty{"presetName"};
    const std::vector<juce::String> PresetManager::excludedParams{ ID::AUTO_GEN_MODE, ID::TEMPO_SYNC_MODE };

    PresetManager::PresetManager(juce::AudioProcessorValueTreeState &_apvts) : apvts(_apvts)
    {
        if (!defaultDirectory.exists())
        {
            const auto result = defaultDirectory.createDirectory();
            if (result.failed())
            {
                DBG("Could not create preset directory: " + result.getErrorMessage());
                jassertfalse;
            }
        }

        apvts.state.addListener(this);
        currentPreset.referTo(apvts.state.getPropertyAsValue(presetNameProperty, nullptr));
    }

    void PresetManager::savePreset(const juce::String &presetName)
    {
        if (presetName.isEmpty())
            return;

        currentPreset.setValue(presetName);
        const auto xmlState = apvts.copyState().createXml();
        const auto presetFile = getPresetFile(presetName);
        if (!xmlState->writeTo(presetFile))
        {
            DBG("Could not create preset file: " + presetFile.getFullPathName());
            jassertfalse;
        }
    }

    void PresetManager::deletePreset(const juce::String &presetName)
    {
        if (presetName.isEmpty())
            return;

        const auto presetFile = getPresetFile(presetName);
        if (!presetFile.existsAsFile())
        {
            DBG("Preset file: " + presetFile.getFullPathName() + " does not exist");
            jassertfalse;
            return;
        }

        if (!presetFile.deleteFile())
        {
            DBG("Preset file: " + presetFile.getFullPathName() + " could not be deleted");
            jassertfalse;
            return;
        }
        currentPreset.setValue("");
    }

    void PresetManager::loadPreset(const juce::String &presetName)
    {
        if (presetName.isEmpty())
            return;

        const auto presetFile = getPresetFile(presetName);
        if (!presetFile.existsAsFile())
        {
            DBG("Preset file: " + presetFile.getFullPathName() + " does not exist");
            jassertfalse;
            return;
        }

        juce::XmlDocument xmlDocument{presetFile};
        auto valueTreeToLoad = juce::ValueTree::fromXml(*xmlDocument.getDocumentElement());
        overrideExcludedParams(valueTreeToLoad);
        apvts.replaceState(valueTreeToLoad);
        currentPreset.setValue(presetName);
    }

    void PresetManager::overrideExcludedParams(juce::ValueTree& valueTree)
    {
        for (auto paramId : excludedParams)
        {
            auto currentValue = apvts.getRawParameterValue(paramId);
            // save value of property to new value tree
        }
    }

    juce::StringArray PresetManager::getAllPresets()
    {
        const auto fileArray = defaultDirectory.findChildFiles(
            juce::File::TypesOfFileToFind::findFiles, false, "*." + extension);
        juce::StringArray presets;
        for (const auto &file : fileArray)
        {
            presets.add(file.getFileNameWithoutExtension());
        }
        return presets;
    }

    //================= private methods =====================

    void PresetManager::valueTreeRedirected(juce::ValueTree &treeWhichHasBeenChanged)
    {
        currentPreset.referTo(treeWhichHasBeenChanged.getPropertyAsValue(presetNameProperty, nullptr));
    }

    juce::File PresetManager::getPresetFile(const juce::String &presetName)
    {
        return defaultDirectory.getChildFile(presetName + "." + extension);
    }
}