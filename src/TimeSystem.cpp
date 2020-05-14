#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Core/Debug/log.hpp"
#include <SDL2/SDL_timer.h>
#include <string>

using namespace Engine::Core::System;
using namespace Engine::Core::Debug;

bool TimeSystem::_limitFPS = false;

double TimeSystem::_time = 0;
double TimeSystem::_temp_time = 0;
double TimeSystem::_time_acc = 0;

unsigned int TimeSystem::_frame_acc =0;

double TimeSystem::_FPSmax = 50.;
double TimeSystem::_sample = 1.; //FPS time sample
double TimeSystem::_deltaTime = 0;
double TimeSystem::_unscaledDeltaTime = 0;
double TimeSystem::_timeScale = 1.;
double TimeSystem::_fixedUnscaledDetlaTime = 1. / 60.;
double TimeSystem::_fixedDetlaTime = _fixedUnscaledDetlaTime * _timeScale;
bool  TimeSystem::_logFPS = true;

void TimeSystem::update()
{	
	_temp_time = SDL_GetTicks() / 1000.0;
	_unscaledDeltaTime = _temp_time - _time;
	_time = _temp_time;

	_time_acc += _unscaledDeltaTime;
	_frame_acc++;

	if (_time_acc >= _sample)
	{
		if (_logFPS)
			std::cout << "FPS: " << round(1 / (_time_acc / _frame_acc)) << std::endl;
		_time_acc = 0;
		_frame_acc = 0;
	}

	_deltaTime = _unscaledDeltaTime * _timeScale;

	if (_limitFPS)
		SDL_Delay((Uint32)(((1. / _FPSmax) * 1000.)-_unscaledDeltaTime));
}

void TimeSystem::update(std::function<void()> fixedUpdateFunction, std::function<void()> updateFunction, std::function<void()> renderFunction)
{
	static int fixedUpdateFrameCount = 0;
	static int unFixedUpdateFrameCount = 0;
	static double chronoFPSLog = 0.;

	_temp_time = SDL_GetTicks() / 1000.0;
	_unscaledDeltaTime = _temp_time - _time;
	_time = _temp_time;

	_time_acc += _unscaledDeltaTime;

	chronoFPSLog += _unscaledDeltaTime;

	if (_logFPS && chronoFPSLog >= _sample)
	{
		std::cout << "FPS (fixedUpdate): " << fixedUpdateFrameCount << std::endl;
		std::cout << "FPS (unFixedUpdate): " << unFixedUpdateFrameCount << std::endl << std::endl;
		chronoFPSLog -= _sample;
		fixedUpdateFrameCount = 0;
		unFixedUpdateFrameCount = 0;
	}

    while (_time_acc >= _fixedUnscaledDetlaTime )
    {
		fixedUpdateFrameCount++;

        fixedUpdateFunction ();
        _time_acc -= _fixedUnscaledDetlaTime;
    }

	unFixedUpdateFrameCount++;
	_deltaTime = _unscaledDeltaTime * _timeScale;

    updateFunction ();
    renderFunction();
}