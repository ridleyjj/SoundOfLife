#pragma once
#include <JuceHeader.h>

namespace jr
{
	class Blinker : public juce::Component, public juce::Timer
	{
		public:
			Blinker() {}
			~Blinker() {}

			/*
			Tells the blinker to blink once. timeInMs is the time for one full period of the blink, i.e. time for on and off to cycle once
			*/
			void startBlink(int timeInMs)
			{
				startTimer(juce::jmax(200, timeInMs / 4));
				blink = true;
				repaint();
			}

			void stopBlink()
			{
				stopTimer();
				blink = false;
				repaint();
			}

			void paint(juce::Graphics& g) override
			{
				if (blink)
				{
					g.setColour(juce::Colours::red);
				}
				else
				{
					g.setColour(juce::Colours::black);
				}

				auto diameter = juce::jmin<float>(getHeight(), getWidth());

				g.fillEllipse(juce::Rectangle<float>(diameter, diameter));
			}

			void timerCallback() override
			{
				stopBlink();
			}

		private:
			bool blink{ false }; // whether blinking state is currently on or off
	};
}
