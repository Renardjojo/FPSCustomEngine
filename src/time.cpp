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


TimeManager::TimeManager (unsigned int FPSmax)
	:	time_		(SDL_GetTicks()),
		dtf_		(0.f),
		FPS_		({{1000}, FPSmax})
{}

void TimeManager::update()
{
 	dtf_ = (time_ > 0) ? ((SDL_GetTicks() - time_) / 1000.f) : 1.0f / 60.0f;
	
	float delay = ((1/(float)FPS_.FPSmax)*1000) - (dtf_*1000);
	
	if(delay <= 0.)
		delay = 1.;

	SDL_Delay(delay);

	dtf_ += delay/1000;

	time_ = SDL_GetTicks();

	FPS_.chrono.update (dtf_);

	if(FPS_.chrono.isEnd())
	{
		FPS_.chrono.reset();
		SLog::log(std::string("FPS : ") + std::to_string(1.f / dtf_) + "\nDelta time : " + std::to_string(dtf_));
	}
}