#include "jr_ScaleManager.h"


namespace jr
{
	ScaleManager::ScaleManager(juce::AudioProcessorValueTreeState& _apvts) : apvts(_apvts)
	{
		baseNoteIndexParameter = dynamic_cast<juce::AudioParameterInt*>(apvts.getParameter(ID::BASE_NOTE));
		baseNoteIndexParameter->addListener(this);
		scaleTypeIndexParameter = dynamic_cast<juce::AudioParameterInt*>(apvts.getParameter(ID::SCALE_TYPE));
		scaleTypeIndexParameter->addListener(this);
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
			noteNumber = chromaticStartIndex + getCurrentBaseNoteIndex() + scaleIndex; // chromatic
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
		scaleIndex = scaleIndex % N;
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

	int ScaleManager::getCellIndexFromNoteNumber(int noteNumber)
	{
		switch (getCurrentScaleTypeIndex())
		{
			case 1:
			{
				// Major
				return getCellIndexFromNoteNumberForScale(ScaleConstants::majIntervals, noteNumber);
			}
			case 2:
			{
				// Minor
				return getCellIndexFromNoteNumberForScale(ScaleConstants::minIntervals, noteNumber);
			}
			default:
			{
				// Chromatic
				noteNumber -= getCurrentBaseNoteIndex();
				noteNumber -= chromaticStartIndex;
				if (noteNumber < -1 || noteNumber > 63)
					noteNumber = -1;
				return noteNumber;
			}
		}
	}

	template<size_t N>
	int ScaleManager::getCellIndexFromNoteNumberForScale(const std::array<int, N> scale, int noteNumber)
	{
		// convert to C scale
		int degree = noteNumber - getCurrentBaseNoteIndex();

		// find octave
		int octave = floor(degree / 12.0f);

		// convert to 0-12
		degree = degree % 12;

		int degreeIndex = -1;

		for (int i = 0; i < scale.size(); i++)
		{
			if (degree == scale.at(i))
			{
				degreeIndex = i;
				break;
			}
		}

		if (degreeIndex == -1)
		{
			return -1;
		}

		int cellIndex = octave * scale.size() + degreeIndex;

		if (cellIndex > 63)
			cellIndex = -1;

		return cellIndex;
	}

	void ScaleManager::parameterValueChanged(int parameterIndex, float newValue)
	{
		currentBaseNoteIndex = apvts.getParameterAsValue(ID::BASE_NOTE).getValue();
		currentScaleTypeIndex = apvts.getParameterAsValue(ID::SCALE_TYPE).getValue();
	}
}