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
#include <vector>
using CellArray = std::vector<jr::CellButton*>;
using Cell2DGrid = std::vector<CellArray*>;


namespace jr
{
    namespace Dimensions
    {
        const int rowSize{ 10 };
    }

    class LifeGrid : public juce::Component
    {
        public:
            LifeGrid();
            ~LifeGrid();

            void resized() override;

        private:
            Cell2DGrid cellGrid;
            const int rowSize{ 25 };
            const int numRows = 25;
    };
}