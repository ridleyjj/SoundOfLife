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

namespace jr
{
    class LifeGrid : public juce::Component
    {
        public:
            LifeGrid();

            void paint(juce::Graphics& g) override;
            void resized() override;

        private:
            CellButton cell;
    };
}