/*
  ==============================================================================

    LifeGrid.h
    Created: 18 Jan 2025 6:37:55pm
    Author:  ridle

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "CellButton.h"
#include "../Service/LifeGridService.h"
#include <vector>
using CellButtonArray = std::vector<jr::CellButton*>;
using CellButton2DGrid = std::vector<CellButtonArray*>;


namespace jr
{
    class LifeGridGUI : public juce::Component, public LifeGridServiceListener
    {
        public:
            LifeGridGUI(LifeGridService& _service);
            ~LifeGridGUI();

            void resized() override;

            void onServiceStateChange() override;

        private:
            LifeGridService& lifeGridService;

            void forEachCell(std::function<void(CellButton*, int, int)> callback);

            CellButton2DGrid cellGrid;
            int rowSize{};
            int numRows{};
    };
}