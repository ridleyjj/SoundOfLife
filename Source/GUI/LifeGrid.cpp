/*
  ==============================================================================

    LifeGrid.cpp
    Created: 18 Jan 2025 6:37:55pm
    Author:  ridle

  ==============================================================================
*/

#include "LifeGrid.h"
#include <JuceHeader.h>

namespace jr
{
    LifeGrid::LifeGrid()
    {
        // populate grid with cells
        for (int rowNum{}; rowNum < numRows; rowNum++)
        {
            auto row = new CellArray();
            for (int i{}; i < rowSize; i++)
            {
                row->push_back(new CellButton());
                addAndMakeVisible(row->at(i));
            }
            cellGrid.push_back(row);
        }
    }

    LifeGrid::~LifeGrid()
    {
        // free memory
        for (int rowNum{}; rowNum < numRows; rowNum++)
        {
            for (int i{}; i < rowSize; i++)
            {
                delete cellGrid.at(rowNum)->at(i);
            }
            delete cellGrid.at(rowNum);
        }
    }

    void LifeGrid::resized()
    {
        auto container = getLocalBounds();
        auto rowHeight = container.proportionOfHeight(1.0f / numRows);
        auto cellWidth = container.proportionOfWidth(1.0f / rowSize);

        for (int rowNum{}; rowNum < numRows; rowNum++)
        {
            auto row = container.removeFromTop(rowHeight);
            for (int cellIndex{}; cellIndex < rowSize; cellIndex++)
            {
                cellGrid.at(rowNum)->at(cellIndex)->setBounds(row.removeFromLeft(cellWidth));
            }
        }
    }
}