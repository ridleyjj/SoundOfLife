/*
  ==============================================================================

    CellButton.h
    Created: 18 Jan 2025 6:37:45pm
    Author:  ridle

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace jr
{
    class CellButtonListener
    {
    public:
        virtual void onCellClick(int m, int n) = 0;
    };

    class CellButton : public juce::Component
    {
        public:
            CellButton(int _rowPos, int _columnPos, CellButtonListener& l) : listener(l)
            {
                row = _rowPos;
                column= _columnPos;
                setMouseCursor(juce::MouseCursor::PointingHandCursor);
            }

            ~CellButton()
            {
                currentHoverComp = nullptr;
            }

            void onClickOrDragOver()
            {
                listener.onCellClick(row, column);
                hasChanged = true;
            }

            void mouseDown(const juce::MouseEvent& event) override
            {
                onClickOrDragOver();
            }

            void mouseUp(const juce::MouseEvent& event) override
            {
                resetHasChanged();
            }
            
            void mouseDrag(const juce::MouseEvent& event) override
            {
                auto globalPos = event.getScreenPosition();
                CellButton* compUnderMouse = dynamic_cast<CellButton*>(juce::Desktop::getInstance().findComponentAt(globalPos));

                bool isCompCell = compUnderMouse != nullptr;

                if (isCompCell && !compUnderMouse->getHasChanged())
                {
                    if (getHasChanged() && compUnderMouse != this)
                    {
                        resetHasChanged();
                    }

                    if (compUnderMouse != currentHoverComp)
                    {
                        if (currentHoverComp != nullptr)
                        {
                            currentHoverComp->resetHasChanged();
                        }

                        currentHoverComp = compUnderMouse;
                    }

                    currentHoverComp->onClickOrDragOver();
                }
            }

            void toggleAlive()
            {
                isAlive = !isAlive;
                repaint();
            }

            void setIsAlive(bool _isAlive)
            {
                isAlive = _isAlive;
            }

            void paint(juce::Graphics& g) override
            {
                g.fillAll(isAlive ? juce::Colours::white : juce::Colours::black);
            }

            bool getIsAlive()
            {
                return isAlive;
            }
            
            bool getHasChanged()
            {
                return hasChanged;
            }
            
            void resetHasChanged()
            {
                hasChanged = false;
            }

        private:
            bool isAlive{ false };
            bool hasChanged{ false };
            int row{};
            int column{};
            CellButtonListener& listener;

            CellButton* currentHoverComp = nullptr;
    };
}