//Project : Engine
//Editing by Six Jonathan
//Date : 27/01/2020 - 17 h 02

#ifndef _GE_TIME_MANAGER_H
#define _GE_TIME_MANAGER_H

#include <algorithm>

namespace Engine::Core::Time
{
	class TimeSystem
	{
	private:
		static bool _limitFPS;
		static double _time;
		static double _temp_time;
		static double _time_acc;

		static unsigned int _frame_acc;

		static float _FPSmax;
		static float  _sample;
		static float _deltaTime;
		static float _unscaledDeltaTime;
		static float _timeScale;

	public:
		TimeSystem() = delete;
		virtual ~TimeSystem() = delete;
		TimeSystem &operator=(TimeSystem const &) = delete;
		TimeSystem &operator=(TimeSystem &&) = delete;

		static void update();

		static float getDeltaTime() { return _deltaTime; }
		static float getUnscaledDetlaTime() { return _unscaledDeltaTime; }
		static float getTimeScale() { return _timeScale; }
		static float setTimeScale(float timeScale) { return _timeScale = timeScale; }
	};

} //namespace Engine::Core::Time

#endif // _GE_TIME_MANAGER_H
