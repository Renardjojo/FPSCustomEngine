//Project : Engine
//Editing by Six Jonathan
//Date : 27/01/2020 - 17 h 02

#ifndef _GE_TIME_MANAGER_H
#define _GE_TIME_MANAGER_H

#include <functional>
#include "GE/Core/Debug/log.hpp"

namespace Engine::Core::System
{
	class TimeSystem
	{
	private:
		static bool _limitFPS;
		static double _time;
		static double _temp_time;
		static double _time_acc;

		static unsigned int _frame_acc;

		static double _FPSmax;
		static double  _sample;
		static double _deltaTime;
		static double _unscaledDeltaTime;
		static double _timeScale;
		static double _fixedDetlaTime;
		static double _fixedUnscaledDetlaTime;
		static bool   _logFPS;
		static float  _scaledTimeAcc;
		static float  _unscaledTimeAcc;

	public:
		TimeSystem() = delete;
		virtual ~TimeSystem() = delete;
		TimeSystem &operator=(TimeSystem const &) = delete;
		TimeSystem &operator=(TimeSystem &&) = delete;

		static void update(); //Only update the time

		/**
		 * @brief Update the time, update system and renderSystem. The update function will call with accumulator to be call a fixed time by second
		 * 
		 * @param fixedUpdateFunction : lambda that contain all fixed update function (that use fixed deltaTime)
		 * @param updateFunction : lambda that contain all update function (physic update, scene, update...)
		 * @param renderFunction : lambda that contain all render function (clear screen, display elements, display screen...)
		 */
		static void update(std::function<void()> fixedUpdateFunction, std::function<void()> updateFunction, std::function<void()> renderFunction);

		static float getFixedDeltaTime() { return static_cast<float>(_fixedDetlaTime); }

		static float getFixedUnscaledDeltaTime() { return static_cast<float>(_fixedUnscaledDetlaTime); }
		static void setFixedUnscaledDeltaTime(double newFixedUnscaledDetlaTime)
		{ 
			if (newFixedUnscaledDetlaTime > 1.)
				Engine::Core::Debug::SLog::logWarning("New fixed unscaled detla time > 1.");

			_fixedUnscaledDetlaTime = newFixedUnscaledDetlaTime;
		}

		static bool getLogFPS() { return _logFPS; }
		static void setLogFPS(bool newLogFPSFlag) { _logFPS = newLogFPSFlag; }

		static float getDeltaTime() { return static_cast<float>(_deltaTime); }
		static float getUnscaledDeltaTime() { return static_cast<float>(_unscaledDeltaTime); }

		static float getAccumulateTime() { return _scaledTimeAcc; }
		static float getAccumulateUnscaledTime() { return _unscaledTimeAcc; }

		static float getTimeScale() { return static_cast<float>(_timeScale); }
		static void setTimeScale(double newtimeScale)
		{ 
			_timeScale = newtimeScale;
			_fixedDetlaTime = _fixedUnscaledDetlaTime * _timeScale;
		}
	};

} //namespace Engine::Core::Time

#endif // _GE_TIME_MANAGER_H
