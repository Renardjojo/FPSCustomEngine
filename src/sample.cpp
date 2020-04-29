#include "GE/Ressources/sample.hpp"
#include "GE/Core/Debug/log.hpp"
#include "GE/Core/Debug/assert.hpp"

using namespace Engine::Ressources;
using namespace Engine::Core::Debug;

Sample::Sample (const char* SamplePath, unsigned int volume)
	: 	Sound		(),
		sample_ 	(nullptr),
		channelUse	(-1)
{
	sample_ = Mix_LoadWAV(SamplePath);
	if(sample_ == nullptr)
	{
		SLog::logError((std::string("Sample cannot be initialize : ") + Mix_GetError()).c_str());
		exit (1);
	}

	setVolume(volume);
}

Sample::Sample (const SampleCreateArg& arg)
	: 	Sound		(),
		sample_ 	(nullptr),
		channelUse	(-1)
{
	sample_ = Mix_LoadWAV(arg.SamplePath);
	if(sample_ == nullptr)
	{
		SLog::logError((std::string("Sample cannot be initialize : ") + Mix_GetError()).c_str());
		exit (1);
	}

	setVolume(arg.volume);
}

Sample::~Sample ()
{
	Mix_FreeChunk(sample_);

}

void Sample::play (int loop) noexcept
{
	channelUse = Mix_PlayChannel(-1, sample_, loop);
	if(channelUse == -1)
	{
		functError(Mix_GetError());
	}
}

void Sample::pause () noexcept
{
	Mix_Pause(channelUse);
}

void Sample::resume () noexcept
{
	Mix_Resume(channelUse);
}

void Sample::halt () noexcept
{
	Mix_HaltChannel(channelUse);
}

void Sample::setVolume (unsigned int volume) noexcept
{
	if(volume > MIX_MAX_VOLUME)
		volume = MIX_MAX_VOLUME;

	Mix_VolumeChunk(sample_, volume);
}

void Sample::pauseAll () noexcept
{
	Mix_Pause(-1); //-1 for general pause
}

void Sample::resumeAll () noexcept
{
	Mix_Resume(-1); //-1 for general resume
}

void Sample::haltAll () noexcept
{
	Mix_HaltChannel(-1); //-1 for general resume
}

void Sample::setGeneralSampleVolume (unsigned int volume) noexcept
{
	if(volume > MIX_MAX_VOLUME)
		volume = MIX_MAX_VOLUME;

	Mix_Volume(-1, volume); //-1 for general volume
	generalSampleVolume = volume;
}

unsigned int Sample::generalSampleVolume 		(DEFAULT_VOLUME);