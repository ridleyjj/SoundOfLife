#include "LifeGridService.h"

namespace jr
{
	const int LifeGridService::numRows{ 8 };
	const int LifeGridService::rowSize{ 8 };

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
		auto calcNextValue = [&](LifeCell* cell, int i, int j)
			{
				bool isAlive = cell->getIsAlive();
				bool isAliveNextGen = getCellNextGeneration(isAlive, i, j);

				// only need to update APVTS if cell state changes
				if (isAlive != isAliveNextGen)
				{
					changedCells.insert(i * rowSize + j);
				}
			};

		forEachCell(calcNextValue);
		notifyListeners(); // listeners need to be notified after new values have all been calculated so that the new values do not interfere with calculations
	}

	void LifeGridService::randomiseSetup()
	{
		auto chooseRandomState = [&](LifeCell* cell, int i, int j)
			{
				bool isAlive = cell->getIsAlive();
				bool isAliveNextGen = random.nextBool();
				if (isAlive != isAliveNextGen)
				{
					changedCells.insert(i * rowSize + j);
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
		auto wrapInt = [](int value, int max)
			{
				if (value < 0)
				{
					value = max + value;
				}
				else if (value >= max)
				{
					value = value - max;
				}
				return value;
			};

		// if neighbours go 'off-grid' assume that they wrap around the grid.
		// i.e. if possible co-ords are from 0-8, -1 becomes 8 and 9 becomes 0

		for (int i{ m - 1 }; i < m + 2; i++)
		{
			int row = wrapInt(i, numRows); // don't reassign i as that would mess up the loop!

			for (int j{ n - 1 }; j < n + 2; j++)
			{
				int column = wrapInt(j, rowSize);

				if (row == m && column == n) // don't count self as neighbour
					continue;

				if (cellGrid.at(row)->at(column)->getIsAlive()) numNeighbours++;
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
			listeners.at(i)->updateCellParam(changedCells);
		}
		changedCells.clear();
	}

	void LifeGridService::clearGrid()
	{
		changedCells.clear();

		auto turnCellOffIfOn = [&](LifeCell* lifeCell, int i, int j)
			{
				if (lifeCell->getIsAlive())
				{
					changedCells.insert(i * rowSize + j);
				}
			};
		forEachCell(turnCellOffIfOn);

		notifyListeners();
	}
}
