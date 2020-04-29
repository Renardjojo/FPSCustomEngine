#include "GE/Resources/sound.hpp"
#include "GE/Core/Debug/log.hpp"
#include "GE/Core/Debug/assert.hpp"

using namespace Engine::Resources;
using namespace Engine::Core::Debug;

Sound::Sound ()
{
    if (numberSound == 0)
        initSDLSoundComponent();
    
    numberSound++;
}

Sound::Sound (const Sound& other)
{
    if (numberSound == 0)
        initSDLSoundComponent();
    
    numberSound++;
}

Sound::~Sound ()
{
    numberSound--;

    if (numberSound == 0)
        releaseSDLSoundComponent();
}

void Sound::setSettingSoundInit	(const SoundSettingArg& arg) noexcept
{
	if (SDLIsInit)
	{
		SLog::logWarning("Sound component already initialize and canno't change it setting");
		return;
	}

	frequency		= arg.frequency;
	format			= arg.format;
	channels		= arg.channels;
	chunksize		= arg.chunksize;
	maskSoundSupport = arg.maskSoundSupport;

}

void Sound::setGeneralVolume (unsigned int volume) noexcept
{
	if(volume > MIX_MAX_VOLUME)
		volume = MIX_MAX_VOLUME;

	Mix_Volume(-1, volume); //-1 for general volume
	generalVolume = volume;
}

void Sound::initSDLSoundComponent 		() noexcept
{
	if((Mix_Init(maskSoundSupport) & maskSoundSupport) != maskSoundSupport)
	{
        SLog::logError((std::string("Mix_Init: Failed to init required mp3 support! ") + Mix_GetError()).c_str());
		exit (1);
	}
	if(Mix_OpenAudio(frequency, format, channels, chunksize) == -1)
	{
        SLog::logError((std::string("SDL_Mixer cannot init Sound component : ") + Mix_GetError()).c_str());
		exit (1);
	}
	
	setGeneralVolume (generalVolume);

	SLog::log("Sound system intialize");
	SDLIsInit = true;
}

void Sound::releaseSDLSoundComponent 	() noexcept
{
	Mix_CloseAudio();
	Mix_Quit();

	SLog::log("Sound system release");
	SDLIsInit = false;
}

bool Sound::SDLIsInit 			    (false);
int Sound::numberSound			    (0);
int Sound::frequency 			    (MIX_DEFAULT_FREQUENCY); 
unsigned int Sound::format		    (MIX_DEFAULT_FORMAT);
int Sound::channels 			    (MIX_DEFAULT_CHANNELS);
int Sound::chunksize 			    (1024);
int Sound::maskSoundSupport         (MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG);
unsigned int Sound::generalVolume 	(DEFAULT_VOLUME);