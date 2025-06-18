#include "jr_PresetManager_Service.h"
#include "id_constants.h"

namespace jr
{
    const juce::File PresetManager::defaultDirectory{
        juce::File::getSpecialLocation(
            juce::File::SpecialLocationType::commonDocumentsDirectory)
            .getChildFile("RidleySound")
            .getChildFile("SoundOfLife")};
    const juce::String PresetManager::extension{"preset"};
    const juce::String PresetManager::underscore{"_"};
    const juce::String PresetManager::presetNameProperty{"presetName"};
    const std::vector<juce::String> PresetManager::excludedParams{ ID::AUTO_GEN_MODE, ID::TEMPO_SYNC_MODE, ID::VELOCITY, ID::FREQUENCY, ID::ACCEPT_MIDI_NOTE_OFF_INPUT, ID::SCALE_TYPE, ID::BASE_NOTE };

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
        const auto presetFile = getUserPresetFile(presetName);
        if (!xmlState->writeTo(presetFile))
        {
            DBG("Could not create preset file: " + presetFile.getFullPathName());
            jassertfalse;
        }
        updateIsUserPreset();
        updateListeners();
    }

    void PresetManager::deletePreset(const juce::String &presetName)
    {
        if (presetName.isEmpty())
            return;

        if (!currentIsUserPreset)
        {
            DBG("Cannot delete non-user presets");
            return;
        }

        const auto presetFile = getUserPresetFile(presetName);
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
        updateIsUserPreset();
        updateListeners();
    }

    void PresetManager::loadPreset(const juce::String &presetName)
    {
        if (presetName.isEmpty())
            return;

        juce::String data;
        auto presetFile = getUserPresetFile(presetName);
        if (presetFile.existsAsFile())
        {
            data = presetFile.loadFileAsString();
        }
        else
        {
            auto binaryData = getBinaryPresetFile(presetName);
            if (binaryData == nullptr)
            {
                DBG("Preset: " + presetName + " does not exist.");
                jassertfalse;
                return;
            }
            data = binaryData;
        }

        juce::XmlDocument xmlDocument{ data };
        
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
            valueTree.appendChild(juce::ValueTree{ "PARAM", {{ "id", paramId }, { "value", (double)*currentValue }} }, nullptr);
        }
    }

    juce::StringArray PresetManager::getAllPresets()
    {
        juce::StringArray presets;
        
        for (int i = 0; i < BinaryData::namedResourceListSize; i++)
        {
            juce::String name = BinaryData::namedResourceList[i];
            if (name.endsWith(extension))
            {
                name = name.replaceCharacter('_', ' ');
                presets.add(name.dropLastCharacters(extension.length() + 1));
            }
        }

        const auto userPresetFiles = defaultDirectory.findChildFiles(
            juce::File::TypesOfFileToFind::findFiles, false, "*." + extension);
        for (const auto &file : userPresetFiles)
        {
            presets.add(file.getFileNameWithoutExtension());
        }

        presets.removeDuplicates(false);
        return presets;
    }

    void PresetManager::removeListener(PresetManager::Listener* listener)
    {
        auto iter = std::remove(listeners.begin(), listeners.end(), listener); // moves any number of matching values to the end of the vector to be removed

        listeners.erase(iter, listeners.end()); // Efficiently erase removed elements
    }

    //================= private methods =====================

    void PresetManager::valueTreeRedirected(juce::ValueTree &treeWhichHasBeenChanged)
    {
        currentPreset.referTo(treeWhichHasBeenChanged.getPropertyAsValue(presetNameProperty, nullptr));
        updateIsUserPreset();
        updateListeners();
    }

    juce::File PresetManager::getUserPresetFile(const juce::String &presetName)
    {
        return defaultDirectory.getChildFile(presetName + "." + extension);  
    }

    const char* PresetManager::getBinaryPresetFile(const juce::String& presetName)
    {
        juce::String binaryDataName = presetName.replaceCharacter(' ', '_') + "_" + extension;
        int size = 0;
        return BinaryData::getNamedResource(binaryDataName.toUTF8(), size);
    }

    void PresetManager::updateIsUserPreset()
    {
        auto val = currentPreset.getValue().toString();
        auto userPresetFile = getUserPresetFile(currentPreset.getValue());
        currentIsUserPreset = userPresetFile.existsAsFile();
    }

    void PresetManager::updateListeners()
    {
        for (int i{}; i < listeners.size(); i++)
        {
            listeners.at(i)->onPresetSelected();
        }
    }
}