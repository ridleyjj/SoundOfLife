#pragma once

#include <JuceHeader.h>
#include "jr_ScaleConstants.h"
#include "id_constants.h"

namespace jr
{
	class ScaleManager
	{
	public:
		ScaleManager(juce::AudioProcessorValueTreeState& _apvts);

		juce::StringArray getAllScaleTypes() { return ScaleConstants::scaleTypes; }

		juce::StringArray getAllBaseNotes() { return ScaleConstants::baseNotes; }
		
		void selectScaleTypeByIndex(const int selectedIndex)
		{ 
			notifyListeners();
			*scaleTypeIndexParameter = selectedIndex;

		}
		
		void selectBaseNoteByIndex(const int selectedIndex)
		{
			notifyListeners();
			*baseNoteIndexParameter = selectedIndex;
		}

		int getCurrentScaleTypeIndex() { return apvts.getParameterAsValue(ID::SCALE_TYPE).getValue(); }
		
		int getCurrentBaseNoteIndex() { return apvts.getParameterAsValue(ID::BASE_NOTE).getValue(); }

		int getNoteNumberFromScaleIndex(int scaleIndex);

		class Listener
		{
			public:
				virtual void beforeScaleModeChanged() = 0;
		};

		void addListener(Listener* listener) { listeners.push_back(listener); }

		void removeListener(Listener* listener);

		/**
		This method returns the cellIndex between 0-63 which is associated with the given MIDI note number.
		If the provided note number is either not in the currently selected scale, or is out of range of the grid, -1 is returned
		*/
		int getCellIndexFromNoteNumber(int noteNumber);
		
	private:
		const int chromaticStartIndex = 12;

		juce::AudioParameterInt* baseNoteIndexParameter;
		juce::AudioParameterInt* scaleTypeIndexParameter;

		void notifyListeners();

        template<size_t N>
		int getNoteNumberForScale(const std::array<int, N> scale, int scaleIndex);

		template<size_t N>
		int getCellIndexFromNoteNumberForScale(const std::array<int, N> scale, int noteNumber);

		std::vector<Listener*> listeners;

		juce::AudioProcessorValueTreeState& apvts;
	};
}