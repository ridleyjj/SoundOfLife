#pragma once

namespace jr
{
	class TimerListener
	{
		public:
			virtual void onTimerBeat() = 0;
	};
}
