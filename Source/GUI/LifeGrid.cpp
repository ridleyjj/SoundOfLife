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
        addAndMakeVisible(cell);
    }

    void LifeGrid::paint(juce::Graphics& g)
    {
        g.fillAll(juce::Colours::red);
    }

    void LifeGrid::resized()
    {
        juce::Rectangle<int> cellSize = juce::Rectangle<int>().withCentre(getBounds().getCentre()).withWidth(20).withHeight(20);
        cell.setBounds(cellSize);
    }
}