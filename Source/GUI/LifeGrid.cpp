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

        hasInit = true;
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

    void LifeGrid::randomiseSetup()
    {
        juce::Random random{};
        for (int i{}; i < numRows; i++)
        {
            for (int j{}; j < rowSize; j++)
            {
                cellGrid.at(i)->at(j)->setNextValue(random.nextBool());
                cellGrid.at(i)->at(j)->triggerGeneration();
            }
        }
        repaint();
    }

    void LifeGrid::nextGeneration()
    {
        if (!hasInit) return;

        for (int i{}; i < numRows; i++)
        {
            for (int j{}; j < rowSize; j++)
            {
                auto isAlive = cellGrid.at(i)->at(j)->getIsAlive();
                cellGrid.at(i)->at(j)->setNextValue(getCellNextGeneration(isAlive, i, j));
            }
        }
        for (int i{}; i < numRows; i++)
        {
            for (int j{}; j < rowSize; j++)
            {
                cellGrid.at(i)->at(j)->triggerGeneration();
            }
        }
        repaint();
    }

    bool LifeGrid::getCellNextGeneration(bool isAlive, int m, int n)
    {
        int numAliveNeighbours = getNumOfAliveNeighbours(m, n);

        if (numAliveNeighbours == 3) // dead or alive, 3 neighbours
        {
            return true;
        }
        else if (isAlive && numAliveNeighbours == 2) // if already alive and 2 neighbours
        {
            return true;
        }

        // else overcrowding or undercrowding results in death
        return false;
    }

    int LifeGrid::getNumOfAliveNeighbours(int m, int n)
    {
        int numNeighbours{ 0 };

        for (int i{ m - 1 }; i < m + 2; i++ )
        {
            if (i >= 0 && i < numRows)
            {
                for (int j{ n - 1 }; j < n + 2; j++)
                {
                    if (j >= 0 && j < rowSize)
                    {
                        if (i == m && j == n) // don't count self as neighbour
                            continue;

                        if (cellGrid.at(i)->at(j)->getIsAlive()) numNeighbours++;
                    }
                }
            }
        }

        return numNeighbours;
    }
}