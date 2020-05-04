#include "GE/Core/TimeSystem/time.hpp"
#include "GE/Core/Debug/log.hpp"
#include <SDL2/SDL_timer.h>
#include <string>

using namespace Engine::Core::Time;
using namespace Engine::Core::Debug;

bool TimeSystem::_limitFPS = false;

double TimeSystem::_time = 0;
double TimeSystem::_temp_time = 0;
double TimeSystem::_time_acc = 0;

unsigned int TimeSystem::_frame_acc =0;

float TimeSystem::_FPSmax =50.f;
float TimeSystem::_sample =1.f; //FPS time sample
float TimeSystem::_deltaTime = 0;
float TimeSystem::_unscaledDeltaTime = 0;
float TimeSystem::_timeScale = 1.f;

void TimeSystem::update()
{	
	_temp_time = SDL_GetTicks() / 1000.0;
	_unscaledDeltaTime = _temp_time - _time;
	_time = _temp_time;

	_time_acc += _unscaledDeltaTime;
	_frame_acc++;

	if (_time_acc >= _sample)
	{
		std::cout << "FPS: " << round(1 / (_time_acc / _frame_acc)) << std::endl;
		_time_acc = 0;
		_frame_acc = 0;
	}

	_deltaTime = _unscaledDeltaTime * _timeScale;

	if (_limitFPS)
		SDL_Delay((Uint32)(((1.f / _FPSmax)*1000)-_unscaledDeltaTime));
}