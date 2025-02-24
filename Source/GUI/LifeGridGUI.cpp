/*
  ==============================================================================

    LifeGrid.cpp
    Created: 18 Jan 2025 6:37:55pm
    Author:  ridle

  ==============================================================================
*/

#include "LifeGridGUI.h"
#include <JuceHeader.h>

namespace jr
{
    LifeGridGUI::LifeGridGUI(LifeGridService& _service) : lifeGridService(_service)
    {
        numRows = lifeGridService.getNumRows();
        rowSize = lifeGridService.getRowSize();

        // populate grid with cells
        for (int rowNum{}; rowNum < numRows; rowNum++)
        {
            auto row = new CellButtonArray();
            for (int i{}; i < rowSize; i++)
            {
                row->push_back(new CellButton(rowNum, i, *this));
                row->at(i)->setIsAlive(lifeGridService.getCell(rowNum, i)->getIsAlive());
                addAndMakeVisible(row->at(i));
            }
            cellGrid.push_back(row);
        }
    }

    LifeGridGUI::~LifeGridGUI()
    {
        // free memory
        for (int rowNum{ numRows - 1 }; rowNum >= 0; rowNum--)
        {
            for (int i{ rowSize - 1 }; i >= 0; i--)
            {
                delete cellGrid.at(rowNum)->at(i);
            }
            delete cellGrid.at(rowNum);
        }
    }

    void LifeGridGUI::onCellClick(int m, int n)
    {
        notifyListenersOnCellClicked(m, n, !lifeGridService.getCell(m, n)->getIsAlive());
    }

    void LifeGridGUI::forEachCell(std::function<void(CellButton*, int, int)> callBack)
    {
        for (int rowNum{}; rowNum < numRows; rowNum++)
        {
            for (int cellIndex{}; cellIndex < rowSize; cellIndex++)
            {
                auto cell = cellGrid.at(rowNum)->at(cellIndex);
                callBack(cell, rowNum, cellIndex);
            }
        }
    }

    void LifeGridGUI::resized()
    {
        auto container = getLocalBounds();

        auto cellSize = juce::jmin(container.proportionOfHeight(1.0 / numRows), container.proportionOfWidth(1.0 / rowSize));

        auto hMargins = container.getWidth() - (cellSize * rowSize);
        auto vMargins = container.getHeight() - (cellSize * numRows);

        container = container.reduced(hMargins / 2.0f, vMargins / 2.0f);

        for (int rowNum{}; rowNum < numRows; rowNum++)
        {
            auto row = container.removeFromTop(cellSize);
            for (int cellIndex{}; cellIndex < rowSize; cellIndex++)
            {
                auto bounds = (cellIndex == rowSize - 1) ? row : row.removeFromLeft(cellSize);
                cellGrid.at(rowNum)->at(cellIndex)->setBounds(bounds);
            }
        }
    }

    void LifeGridGUI::updateCellIsAlive(CellButton* cell, bool isAlive)
    {
        jassert(cell != nullptr);

        cell->setIsAlive(isAlive);
        repaint();
    }

    void LifeGridGUI::removeListener(LifeGridGUIListener* l)
    {
        auto iter = std::remove(listeners.begin(), listeners.end(), l); // moves any number of matching values to the end of the vector to be removed

        listeners.erase(iter, listeners.end()); // Efficiently erase removed elements
    }

    void LifeGridGUI::notifyListenersOnCellClicked(int m, int n, bool isAlive)
    {
        for (int i{}; i < listeners.size(); i++)
        {
            listeners.at(i)->onLifeGridCellClicked(m, n, isAlive);
        }
    }
}