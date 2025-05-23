#pragma once
#include "LifeGridGUI.h"
#include <JuceHeader.h>


namespace jr
{
	class LifeGridAttachment : public LifeGridGUIListener
	{
		public:
			LifeGridAttachment(LifeGridGUI& l, juce::AudioProcessorValueTreeState& params);

			~LifeGridAttachment();

			/*
			Update LifeGridService based on new GUI state
			*/
			void onLifeGridCellClicked(int m, int n, bool isAlive) override;

		private:
			/*
			Returns update cell function to be passed to each parameter attachment. Update cell is calculated 
			*/
			std::function<void(float)> getUpdateCellMethod(int m, int n);

			std::vector<std::shared_ptr<juce::ParameterAttachment>> paramAttachments{};
			
			LifeGridGUI& lifeGrid;
	};
}
