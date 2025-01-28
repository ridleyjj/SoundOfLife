#include "LifeGridAttachment.h"
#include "../PluginProcessor.h"

namespace jr
{
	LifeGridAttachment::LifeGridAttachment(LifeGridGUI& l, juce::AudioProcessorValueTreeState& params, juce::UndoManager* undoManager) : lifeGrid(l)
	{
		int numRows = lifeGrid.getNumRows();
		int rowSize = lifeGrid.getRowSize();
		int m{};
		int n{};

		for (int i{}; i < numRows * rowSize; i++)
		{
			auto param = params.getParameter(ID::getCellId(i));
			paramAttachments.push_back(std::make_shared<juce::ParameterAttachment>(param, getUpdateCellMethod(m, n), undoManager));
			paramAttachments.at(i)->sendInitialUpdate();

			if (n++ >= rowSize)
			{
				n = 0;
				m++;

				if (m >= numRows)
				{
					break;
				}
			}
		}

		lifeGrid.addListener(this);
	}

	LifeGridAttachment::~LifeGridAttachment()
	{
		lifeGrid.removeListener(this);
	}

	void LifeGridAttachment::onLifeGridCellClicked(int m, int n, bool isAlive)
	{
		int i = (lifeGrid.getNumRows() * m) + n;
		paramAttachments.at(i)->setValueAsCompleteGesture(isAlive ? 1.0f : 0.0f);
	}

	std::function<void(float)> LifeGridAttachment::getUpdateCellMethod(int m, int n)
	{
		return [&](float newValue)
			{
				bool isAlive = newValue > 0.1f;
				lifeGrid.updateCellIsAlive(m, n, isAlive);
			};
	}
}