#pragma once

#include <JuceHeader.h>
#include "../Service/jr_ScaleManager.h"

namespace jr
{
	class ScalePanel : public juce::Component, juce::ComboBox::Listener
	{
	public:
		ScalePanel(ScaleManager& sm) : scaleManager(sm)
		{
			configureComboBox(noteSelectionBox);
			configureComboBox(scaleSelectionBox);

			loadNoteList();

			loadScaleList();
		}

		void resized() override
		{
			const int margin = 8;
			const auto container = getLocalBounds().reduced(margin);
			auto bounds = container;

			noteSelectionBox.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.35f)).reduced(margin));
			scaleSelectionBox.setBounds(bounds.reduced(margin));
		}

	private:

		void comboBoxChanged(juce::ComboBox* comboBox) override
		{
			if (comboBox == &noteSelectionBox)
			{
				scaleManager.selectBaseNoteByIndex(comboBox->getSelectedItemIndex());
			}
			else if (comboBox == &scaleSelectionBox)
			{
				scaleManager.selectScaleTypeByIndex(comboBox->getSelectedItemIndex());
			}
		}

		void loadNoteList()
		{
			noteSelectionBox.clear(juce::dontSendNotification);
			noteSelectionBox.addItemList(scaleManager.getAllBaseNotes(), 1);
			noteSelectionBox.setSelectedItemIndex(scaleManager.getCurrentBaseNoteIndex(), juce::dontSendNotification);
		}

		void loadScaleList()
		{
			scaleSelectionBox.clear(juce::dontSendNotification);
			scaleSelectionBox.addItemList(scaleManager.getAllScaleTypes(), 1);
			scaleSelectionBox.setSelectedItemIndex(scaleManager.getCurrentScaleTypeIndex(), juce::dontSendNotification);
		}

		void configureComboBox(juce::ComboBox& comboBox)
		{
			comboBox.setMouseCursor(juce::MouseCursor::PointingHandCursor);
			addAndMakeVisible(comboBox);
			comboBox.addListener(this);
		}

		ScaleManager& scaleManager;

		juce::ComboBox noteSelectionBox;

		juce::ComboBox scaleSelectionBox;
	};
}
