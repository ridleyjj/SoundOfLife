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
			Starts the blinker blinking. timeInMs is the time for one full period of the blink, i.e. time for on and off to clycle once
			*/
			void startBlink(int timeInMs)
			{
				startTimer(timeInMs / 2);
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

				g.fillAll();
			}

			void timerCallback() override
			{
				blink = !blink;
				repaint();
			}

		private:
			bool blink{ false }; // whether blinking state is currently on or off
	};
}
