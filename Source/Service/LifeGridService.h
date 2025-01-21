#pragma once
#include <JuceHeader.h>
#include <vector>
using CellArray = std::vector<jr::LifeGridService::LifeCell*>;
using Cell2DGrid = std::vector<CellArray*>;


namespace jr
{
	class LifeGridService
	{
		public:
			LifeGridService();
			~LifeGridService();

            void nextGeneration();

            void randomiseSetup();

            int getNumRows() { return numRows; }
            int getRowSize() { return rowSize; }
            
            CellButton* getCell(int m, int n) { return cellGrid.at(m)->at(n); }

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

		private:
            bool getCellNextGeneration(bool isAlive, int m, int n);
            int getNumOfAliveNeighbours(int m, int n);

            void forEachCell(std::function<void(LifeCell*, int, int)> callback);

            Cell2DGrid cellGrid;
            const int rowSize{ 9 };
            const int numRows = 9;

            juce::Random random{};
	};
}
