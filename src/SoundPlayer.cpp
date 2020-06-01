#include "GE/Ressources/SoundPlayer.hpp"
#include "GE/Ressources/Sound.hpp"

#include <string.h>

using namespace Engine::Ressources;
using namespace Engine::Core::Debug;

bool SoundPlayer::_initialized = false;
ALCdevice *SoundPlayer::_device;
ALCcontext *SoundPlayer::_context;
std::vector<std::string> SoundPlayer::_devices;

bool SoundPlayer::initialize()
{
    _device = alcOpenDevice(NULL);
    if (!_device)
    {
        SLog::logError("Device not created");
        SoundPlayer::_initialized = false;
        return false;
    }

    _context = alcCreateContext(_device, NULL);
    if (!_context)
    {
        SLog::logError("Context not created");
        SoundPlayer::_initialized = false;
        return false;
    }

    if (!alcMakeContextCurrent(_context))
    {
        SLog::logError("Make current context fail");
        SoundPlayer::_initialized = false;
        return false;
    }
    alutInitWithoutContext(NULL, NULL);

    //                          lookat          up
    ALfloat Orientation[] = {0.f, 0.f, 1.f, 0.f, 1.f, 0.f};
    alListener3f(AL_POSITION, 0.f, 0.f, 0.f);
    alListener3f(AL_VELOCITY, 0.f, 0.f, 0.f);
    alListenerfv(AL_ORIENTATION, Orientation);

    SoundPlayer::_initialized = true;
    return true;
}

void SoundPlayer::destroy()
{
    alcMakeContextCurrent(NULL);
    alcDestroyContext(_context);
    alcCloseDevice(_device);
    alutExit();
}

void SoundPlayer::listDevices()
{
    _devices.clear();

    const ALCchar *_deviceList = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
    std::cout << "Audio _devices :" << std::endl;
    if (_deviceList)
    {
        while (strlen(_deviceList) > 0)
        {
            std::cout << "\t" << _deviceList << std::endl;
            _devices.push_back(_deviceList);
            _deviceList += strlen(_deviceList) + 1;
        }
    }
}
void SoundPlayer::play(Sound &sound)
{
    if (!_initialized)
        SLog::logError("Sound Player is not _initialized");

    alSourcePlay(sound.getSource());
}
void SoundPlayer::stop(Sound &sound)
{
    if (!_initialized)
        SLog::logError("Sound Player is not _initialized");

    alSourceStop(sound.getSource());
}
void SoundPlayer::pause(Sound &sound)
{
    if (!_initialized)
        SLog::logError("Sound Player is not _initialized");

    alSourcePause(sound.getSource());
}
void SoundPlayer::rewind(Sound &sound)
{
    if (!_initialized)
        SLog::logError("Sound Player is not _initialized");

    alSourceRewind(sound.getSource());
}