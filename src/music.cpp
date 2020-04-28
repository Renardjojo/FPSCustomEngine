#include "music.hpp"

#include "log.hpp"
#include "assert.hpp"

using namespace Engine::Resources;
using namespace Engine::Core::Debug;

Music::Music (const char* musicPath, unsigned int volume)
	: 	Sound		(),
		music_ 	(nullptr)
{
	music_ = Mix_LoadMUS(musicPath);
	if(music_ == nullptr)
	{
		SLog::logError((std::string("Music cannot be initialize : ") + Mix_GetError()).c_str());
		exit (1);
	}

	setVolume(volume);
}

Music::Music (const MusicCreateArg& arg)
	: 	Sound		(),
		music_ 		(nullptr)
{
	music_ = Mix_LoadMUS(arg.MusicPath);
	if(music_ == nullptr)
	{
		SLog::logError((std::string("Music cannot be initialize : ") + Mix_GetError()).c_str());
		exit (1);
	}

	setVolume(arg.volume);
}

Music::~Music ()
{
	Mix_FreeMusic(music_);
}

void Music::play (int loop) noexcept
{
	if(Mix_PlayMusic(music_, loop) == -1)
	{
		functError(Mix_GetError());
	}
}

void Music::pause () noexcept
{
	Mix_PauseMusic();
}

void Music::resume () noexcept
{
	Mix_ResumeMusic();
}

void Music::halt () noexcept
{
	Mix_HaltMusic();
}

void Music::setVolume (unsigned int volume) noexcept
{
	if(volume > MIX_MAX_VOLUME)
		volume = MIX_MAX_VOLUME;

	Mix_VolumeMusic(volume);
}