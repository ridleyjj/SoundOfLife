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

	private:
		juce::AudioParameterInt* baseNoteIndexParameter;
		juce::AudioParameterInt* scaleTypeIndexParameter;

		void notifyListeners();

        template<size_t N>
		int getNoteNumberForScale(const std::array<int, N> scale, int scaleIndex);

		std::vector<Listener*> listeners;

		juce::AudioProcessorValueTreeState& apvts;
	};
}