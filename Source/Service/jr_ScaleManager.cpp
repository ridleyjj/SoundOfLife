#include "jr_ScaleManager.h"


namespace jr
{
	ScaleManager::ScaleManager(juce::AudioProcessorValueTreeState& _apvts) : apvts(_apvts)
	{
		baseNoteIndexParameter = dynamic_cast<juce::AudioParameterInt*>(apvts.getParameter(ID::BASE_NOTE));
		scaleTypeIndexParameter = dynamic_cast<juce::AudioParameterInt*>(apvts.getParameter(ID::SCALE_TYPE));
	}

	int ScaleManager::getNoteNumberFromScaleIndex(int scaleIndex)
	{
		juce::String currentScaleType = ScaleConstants::scaleTypes[getCurrentScaleTypeIndex()];

		int noteNumber{};

		switch (getCurrentScaleTypeIndex())
		{
		case 1:
			noteNumber = getNoteNumberForScale(ScaleConstants::majIntervals, scaleIndex);
			break;
		case 2:
			noteNumber = getNoteNumberForScale(ScaleConstants::minIntervals, scaleIndex);
			break;
		default:
			noteNumber = 12 + getCurrentBaseNoteIndex() + scaleIndex; // chromatic
			break;
		}

		return noteNumber;
	}

	template<size_t N>
	int ScaleManager::getNoteNumberForScale(const std::array<int, N> scale, int scaleIndex)
	{
		int baseNote = getCurrentBaseNoteIndex();
		int octave = floor(scaleIndex / N);
		octave = octave % 10; // wrap octaves to stay in MIDI range
		scaleIndex = scaleIndex % (N - 1);
		int noteNumber = baseNote + (octave * 12) + scale[scaleIndex];

		// only return valid MIDI
		while (noteNumber > 127)
		{
			noteNumber -= 12;
		}

		return noteNumber;
	}

	void ScaleManager::removeListener(ScaleManager::Listener* listener)
	{
		auto iter = std::remove(listeners.begin(), listeners.end(), listener);
		listeners.erase(iter, listeners.end());
	}

	void ScaleManager::notifyListeners()
	{
		for (Listener* l : listeners)
		{
			l->beforeScaleModeChanged();
		}
	}
}