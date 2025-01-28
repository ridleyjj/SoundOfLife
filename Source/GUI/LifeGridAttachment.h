#pragma once
#include "LifeGridGUI.h"
#include <JuceHeader.h>


namespace jr
{
	class LifeGridAttachment : public LifeGridGUIListener
	{
		public:
			/*
			params is a reference to the group of AudioParameterBools which are a flattened 1D array of all of the cells in the grid
			*/
			LifeGridAttachment(LifeGridGUI& l, juce::AudioProcessorValueTreeState& params) : lifeGrid(l) { LifeGridAttachment(l, params, nullptr); }

			/*
			params is a reference to the group of AudioParameterBools which are a flattened 1D array of all of the cells in the grid
			*/
			LifeGridAttachment(LifeGridGUI& l, juce::AudioProcessorValueTreeState& params, juce::UndoManager* undoManager);

			~LifeGridAttachment();

			/*
			Update LifeGridService based on new GUI state
			*/
			void onLifeGridCellClicked(int m, int n, bool isAlive) override;

		private:
			// 1D array of juce::ParameterAttachment objects? One for each cell
			std::vector<std::shared_ptr<juce::ParameterAttachment>> paramAttachments{};

			LifeGridGUI& lifeGrid;
			
			/*
			Returns update cell function to be passed to each parameter attachment. Update cell is calculated 
			*/
			std::function<void(float)> getUpdateCellMethod(int m, int n);
	};
}
