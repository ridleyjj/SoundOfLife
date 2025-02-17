#pragma once
#include <JuceHeader.h>

namespace jr
{
	class ToggleButtonAttachment : public juce::ToggleButton::Listener
	{
	public:
		ToggleButtonAttachment(juce::ToggleButton& _button, juce::String paramId, juce::AudioProcessorValueTreeState& apvts): button(_button)
		{
			paramAttachment = std::make_unique<juce::ParameterAttachment>(*apvts.getParameter(paramId), paramCallback, nullptr);

			button.addListener(this);

			paramAttachment->sendInitialUpdate();
		}

		~ToggleButtonAttachment()
		{
			button.removeListener(this);
		}

		void buttonClicked(juce::Button* b) override {}

		void buttonStateChanged(juce::Button* b) override
		{
			if (b == &button)
			{
				paramAttachment->setValueAsCompleteGesture(button.getToggleState());
			}
		}

	private:
		std::unique_ptr<juce::ParameterAttachment> paramAttachment;
		juce::ToggleButton& button;

		std::function<void(float)> paramCallback = [&](float val)
			{
				button.setToggleState((bool)val, juce::dontSendNotification);
			};
	};
}
