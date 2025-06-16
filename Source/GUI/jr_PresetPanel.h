#pragma once

#include <JuceHeader.h>
#include "../Service/jr_PresetManager_Service.h"

namespace jr
{

    class PresetPanel : public juce::Component, juce::Button::Listener, juce::ComboBox::Listener, PresetManager::Listener
    {
    public:
        PresetPanel(PresetManager &pm) : presetManager(pm)
        {
            addAndMakeVisible(label);

            configureButton(saveButton, "Save");
            configureButton(deleteButton, "Delete");

            presetList.setTextWhenNothingSelected("No Preset Selected");
            presetList.setMouseCursor(juce::MouseCursor::PointingHandCursor);
            addAndMakeVisible(presetList);
            presetList.addListener(this);
            presetManager.addListener(this);

            loadPresetList();

            onPresetSelected();
        }

        ~PresetPanel()
        {
            saveButton.removeListener(this);
            deleteButton.removeListener(this);
            presetList.removeListener(this);
            presetManager.removeListener(this);
        }

        void resized() override
        {
            const int xMargin = 3;
            const int yMargin = 4;
            const auto container = getLocalBounds().reduced(xMargin);
            auto bounds = container;

            label.setBounds(bounds.removeFromTop(20));

            presetList.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.6f)).reduced(xMargin, yMargin));

            saveButton.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.2f)).reduced(xMargin, yMargin));

            deleteButton.setBounds(bounds.reduced(xMargin, yMargin));
        }

        void onPresetSelected() override
        {
            deleteButton.setEnabled(presetManager.getIsCurrentPresetUserPreset());
            deleteButton.repaint();
        }

        void paint(juce::Graphics& g) override
        {
            g.setColour(juce::Colours::black);
            g.drawRect(getLocalBounds().reduced(1), 2.0f);
        }

    private:
        void loadPresetList()
        {
            const auto allPresets = presetManager.getAllPresets();
            presetList.clear(juce::dontSendNotification);
            presetList.addItemList(allPresets, 1);
            presetList.setSelectedItemIndex(allPresets.indexOf(presetManager.getCurrentPreset()), juce::dontSendNotification);
        }

        void buttonClicked(juce::Button *button) override
        {
            if (button == &saveButton)
                onSaveButtonClicked();

            if (button == &deleteButton)
                onDeleteButtonClicked();
        };

        void onSaveButtonClicked()
        {
            fileChooser = std::make_unique<juce::FileChooser>(
                "Please enter the name of the preset to save",
                PresetManager::defaultDirectory,
                "*." + PresetManager::extension);
            fileChooser->launchAsync(juce::FileBrowserComponent::saveMode, [&](const juce::FileChooser &chooser)
                                     {
                const auto resultFile = chooser.getResult();
                presetManager.savePreset(resultFile.getFileNameWithoutExtension()); 
                loadPresetList(); });
        }

        void onDeleteButtonClicked()
        {
            presetManager.deletePreset(presetManager.getCurrentPreset());
            loadPresetList();
        }

        void comboBoxChanged(juce::ComboBox *comboBox) override
        {
            if (comboBox == &presetList)
            {
                presetManager.loadPreset(presetList.getItemText(presetList.getSelectedItemIndex()));
            }
        };

        void configureButton(juce::Button &button, const juce::String &buttonText)
        {
            button.setButtonText(buttonText);
            button.setMouseCursor(juce::MouseCursor::PointingHandCursor);
            addAndMakeVisible(button);
            button.addListener(this);
        }

        jr::PresetManager &presetManager;

        juce::TextButton saveButton, deleteButton;
        juce::ComboBox presetList;
        std::unique_ptr<juce::FileChooser> fileChooser;

        juce::Rectangle<int> deleteButtonBounds;

        juce::Label label{ "presetPanelLabel", "Presets" };

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetPanel);
    };
}