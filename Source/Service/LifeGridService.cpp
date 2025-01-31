#include "LifeGridService.h"

namespace jr
{
	const int LifeGridService::numRows{ 9 };
	const int LifeGridService::rowSize{ 9 };

	LifeGridService::LifeGridService()
	{
		// populate grid with cells
		for (int rowNum{}; rowNum < numRows; rowNum++)
		{
			auto row = new CellArray();
			for (int i{}; i < rowSize; i++)
			{
				row->push_back(new LifeCell());
			}
			cellGrid.push_back(row);
		}
	}

	LifeGridService::~LifeGridService()
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

	void LifeGridService::nextGeneration()
	{
		changedCells.clear();

		auto calcNextValue = [&](LifeCell* cell, int i, int j)
			{
				bool isAlive = cell->getIsAlive();
				bool isAliveNextGen = getCellNextGeneration(isAlive, i, j);

				// only need to update APVTS if cell state changes
				if (isAlive != isAliveNextGen)
				{
					changedCells.push_back(i * rowSize + j);
				}
			};

		forEachCell(calcNextValue);
		notifyListeners(); // listeners need to be notified after new values have all been calculated so that the new values do not interfere with calculations
	}

	void LifeGridService::randomiseSetup()
	{
		changedCells.clear();

		auto chooseRandomState = [&](LifeCell* cell, int i, int j)
			{
				bool isAlive = cell->getIsAlive();
				bool isAliveNextGen = random.nextBool();
				if (isAlive != isAliveNextGen)
				{
					changedCells.push_back(i * rowSize + j);
				}
			};
		forEachCell(chooseRandomState);

		notifyListeners();
	}

	bool LifeGridService::getCellNextGeneration(bool isAlive, int m, int n)
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

	int LifeGridService::getNumOfAliveNeighbours(int m, int n)
	{
		int numNeighbours{ 0 };

		for (int i{ m - 1 }; i < m + 2; i++)
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

	void LifeGridService::forEachCell(std::function<void(LifeCell* cell, int m, int n)> callback)
	{
		for (int rowNum{}; rowNum < numRows; rowNum++)
		{
			for (int cellIndex{}; cellIndex < rowSize; cellIndex++)
			{
				auto cell = cellGrid.at(rowNum)->at(cellIndex);
				callback(cell, rowNum, cellIndex);
			}
		}
	}

	void LifeGridService::addListener(LifeGridServiceListener* l)
	{
		listeners.push_back(l);
	}

	void LifeGridService::removeListener(LifeGridServiceListener* l)
	{
		auto iter = std::remove(listeners.begin(), listeners.end(), l);
		listeners.erase(iter, listeners.end());
	}

	void LifeGridService::notifyListeners()
	{
		for (int i{}; i < listeners.size(); i++)
		{
			for (int cellIndex : changedCells)
			{
				listeners.at(i)->updateCellParam(cellIndex);
			}
		}
		changedCells.clear();
	}
}
