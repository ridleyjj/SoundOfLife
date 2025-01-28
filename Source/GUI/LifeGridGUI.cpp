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
        // old method - updating directly through service reference
        //lifeGridService.getCell(m, n)->toggleAlive();
        //cellGrid.at(m)->at(n)->setIsAlive(lifeGridService.getCell(m, n)->getIsAlive());

        // new method - updating parameter attachment to send update via APVTS
        notifyListenersOnCellClicked(m, n, !lifeGridService.getCell(m, n)->getIsAlive());
        
        repaint();
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

    void LifeGridGUI::onServiceStateChange()
    {
        /*auto updateCellValue = [&](CellButton* cell, int m, int n)
            {
                auto serviceCell = lifeGridService.getCell(m, n);
                if (serviceCell != nullptr)
                {
                    cell->setIsAlive(serviceCell->getIsAlive());
                }
            };
        forEachCell(updateCellValue);
        repaint();*/
    }

    void LifeGridGUI::updateCellIsAlive(int m, int n, bool isAlive)
    {
        jassert(m > 0 && m < numRows);
        jassert(n > 0 && n < rowSize);

        cellGrid.at(m)->at(n)->setIsAlive(isAlive);
        repaint();
    }

    void LifeGridGUI::removeListener(LifeGridGUIListener* l)
    {
        for (int i{}; i < listeners.size(); i++)
        {
            if (l == listeners.at(i).get())
            {
                listeners.erase(listeners.begin() + i);
                return;
            }
        }
    }

    void LifeGridGUI::notifyListenersOnCellClicked(int m, int n, bool isAlive)
    {
        for (int i{}; i < listeners.size(); i++)
        {
            listeners.at(i)->onLifeGridCellClicked(m, n, isAlive);
        }
    }
}