#pragma once
#include <JuceHeader.h>
#include <vector>

namespace jr
{
    class LifeGridServiceListener
    {
        public:
            virtual void onServiceStateChange() = 0;
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

            void addListener(std::shared_ptr<LifeGridServiceListener> gridUI);
            void removeListener(std::shared_ptr<LifeGridServiceListener> gridUI);

		private:
            bool getCellNextGeneration(bool isAlive, int m, int n);
            int getNumOfAliveNeighbours(int m, int n);

            void notifyListeners();

            void forEachCell(std::function<void(LifeCell*, int, int)> callback);

            Cell2DGrid cellGrid;
            //const int rowSize{ 9 };
            //const int numRows = 9;

            juce::Random random{};

            std::vector <std::shared_ptr< LifeGridServiceListener >> listeners{};
	};
}
