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
    class LifeGrid : public juce::Component
    {
        public:
            LifeGrid();
            ~LifeGrid();

            void resized() override;

            void nextGeneration();

            void randomiseSetup();

        private:
            bool getCellNextGeneration(bool isAlive, int m, int n);
            int getNumOfAliveNeighbours(int m, int n);

            Cell2DGrid cellGrid;
            const int rowSize{ 25 };
            const int numRows = 25;

            bool hasInit{ false };
    };
}