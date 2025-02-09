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

            void mouseUp(const juce::MouseEvent& event) override
            {
                listener.onCellClick(row, column);
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

        private:
            bool isAlive{ false };
            int row{};
            int column{};
            CellButtonListener& listener;
    };
}