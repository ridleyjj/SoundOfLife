#include "id_constants.h"

juce::String ID::getCellId(int index)
{
    return "CELL_" + juce::String{ index };
}
