#pragma once
#include "LifeGridGUI.h"


namespace jr
{
	class LifeGridAttachment : public LifeGridGUIListener
	{
		public:
			LifeGridAttachment();

			/*
			Update LifeGridService based on new GUI state
			*/
			void onLifeGridStateChange() override;

		private:
			// 2D array of juce::ParameterAttachment objects? One for each cell

			LifeGridGUI& lifeGrid;
			
			/*
			Update cell function to be passed to each parameter attachment
			*/
			std::function<void(bool)> getUpdateCellMethod(int m, int n)
			{
				return [&](bool newValue)
					{
						// use m and n to identify with cell to update 
						return;
					};
			};
	};
}
