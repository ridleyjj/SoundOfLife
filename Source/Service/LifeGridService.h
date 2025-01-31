#pragma once
#include <JuceHeader.h>
#include <vector>

namespace jr
{
    class LifeGridServiceListener
    {
        public:
            virtual void updateCellParam(int cellIndex) = 0;
    };


    class LifeCell
    {
    public:

        void toggleAlive()
        {
            isAlive = !isAlive;
        }

        void setNextValue(bool _isAlive)
        {
            nextValue = _isAlive;
        }

        void triggerGeneration()
        {
            isAlive = nextValue;
        }


        bool getIsAlive()
        {
            return isAlive;
        }

    private:
        bool isAlive{ false };
        bool nextValue{ false };
    };
    using CellArray = std::vector<LifeCell*>;
    using Cell2DGrid = std::vector<CellArray*>;

	class LifeGridService
	{
		public:
			LifeGridService();
			~LifeGridService();

            static const int numRows;
            static const int rowSize;

            void nextGeneration();

            void randomiseSetup();

            int getNumRows() { return numRows; }
            int getRowSize() { return rowSize; }

            LifeCell* getCell(int m, int n) { return cellGrid.at(m)->at(n); }

            void addListener(LifeGridServiceListener* l);
            void removeListener(LifeGridServiceListener* l);

		private:
            bool getCellNextGeneration(bool isAlive, int m, int n);
            int getNumOfAliveNeighbours(int m, int n);

            /*
            Uses changedCells vector to update listeners of all cells that have changed in current gesture
            */
            void notifyListeners();

            void forEachCell(std::function<void(LifeCell*, int, int)> callback);

            Cell2DGrid cellGrid;
            std::vector<int> changedCells{};

            juce::Random random{};

            std::vector <LifeGridServiceListener*> listeners{};
	};
}
