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
    class LifeGridGUIListener
    {
        public:
            virtual void onLifeGridCellClicked(int m, int n, bool isAlive) = 0;
    };

    class LifeGridGUI : public juce::Component, public CellButtonListener
    {
        public:
            LifeGridGUI(LifeGridService& _service);
            ~LifeGridGUI();

            void resized() override;

            void paint(juce::Graphics& g) override;

            void onCellClick(int m, int n) override;

            void updateCellIsAlive(CellButton* cell, bool isAlive);

            void addListener(LifeGridGUIListener* l) { listeners.push_back(l); }

            void removeListener(LifeGridGUIListener* l);

            int getNumRows() { return numRows; }
            
            int getRowSize() { return rowSize; }

            jr::CellButton* getCell(int m, int n) { return cellGrid.at(m)->at(n); }

        private:
            LifeGridService& lifeGridService;

            void forEachCell(std::function<void(CellButton*, int, int)> callback);

            void notifyListenersOnCellClicked(int m, int n, bool isAlive);

            CellButton2DGrid cellGrid;
            int rowSize{};
            int numRows{};

            juce::Rectangle<int> outlineBounds{};

            std::vector<LifeGridGUIListener*> listeners;
    };
}