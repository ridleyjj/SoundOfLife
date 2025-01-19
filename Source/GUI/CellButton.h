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
    class CellButton : public juce::Component
    {
        public:
            CellButton()
            {
                setMouseCursor(juce::MouseCursor::PointingHandCursor);
            }

            void mouseUp(const juce::MouseEvent& event) override
            {
                toggleAlive();
            }

            void toggleAlive()
            {
                isAlive = !isAlive;
                repaint();
            }

            void paint(juce::Graphics& g) override
            {
                g.fillAll(isAlive ? juce::Colours::white : juce::Colours::black);
            }

        private:
            bool isAlive{ false };
    };
}