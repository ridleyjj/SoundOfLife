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
using CellArray = std::vector<jr::CellButton*>;
using Cell2DGrid = std::vector<CellArray*>;


namespace jr
{
    class LifeGridGUI : public juce::Component
    {
        public:
            LifeGridGUI(LifeGridService& _service);
            ~LifeGridGUI();

            void resized() override;

        private:
            LifeGridService& lifeGridService;

            void forEachCell(std::function<void(CellButton*, int, int)> callback);

            Cell2DGrid cellGrid;
            int rowSize{};
            int numRows{};
    };
}