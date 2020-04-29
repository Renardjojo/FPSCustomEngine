#include "GE/Core/TimeSystem/time.hpp"
#include "GE/Core/Debug/log.hpp"
#include <SDL2/SDL_timer.h>
#include <string>

using namespace Engine::Core::TimeSystem;
using namespace Engine::Core::Debug;

ChronoDt::ChronoDt (float delay)
	:	delay_	(delay/1000),
		time_	(0.),
		isFinish_(false)
{}

ChronoDt::~ChronoDt ()
{}

void ChronoDt::update(float delta_time)
{
	isFinish_ = time_ > delay_;

	if (time_ > delay_)
		return;

	time_ += delta_time;
}

bool ChronoDt::isEnd() const
{
	return isFinish_;
}

void ChronoDt::reset()
{
	isFinish_ 	= false;
	time_ 		= 0;
}

void TimeSystem::update()
{
 	unscaledDetlaTime_ = (timeExFrame_ > 0) ? ((SDL_GetTicks() - timeExFrame_) / 1000.f) : 1.0f / 60.0f;
	
	float delay = ((1/(float)FPS_.FPSmax)*1000) - (unscaledDetlaTime_*1000);
	
	if(delay <= 0.)
		delay = 1.;

	SDL_Delay(delay);

	unscaledDetlaTime_ += delay/1000;
	deltaTime_ = unscaledDetlaTime_ * timeScale_;

	timeExFrame_ = SDL_GetTicks();

	FPS_.chrono.update (unscaledDetlaTime_);

	if(FPS_.chrono.isEnd())
	{
		FPS_.chrono.reset();
		SLog::log(std::string("FPS : ") + std::to_string(1.f / unscaledDetlaTime_));
	}
}

float TimeSystem::deltaTime_			= 0.f;
float TimeSystem::unscaledDetlaTime_ 	= 0.f;
float TimeSystem::timeScale_			= 1.f;
float TimeSystem::timeExFrame_			= 0.f;
FPS   TimeSystem::FPS_					= {{1000}, DEFAULT_FPS_MAX};