#include "GE/Ressources/Sound.hpp"

using namespace Engine::Ressources;

Sound::Sound(const char *path)
{
    alGenSources((ALuint)1, &_source);
    alGenBuffers((ALuint)1, &_buffer);

    setPitch(1.f);
    setGain(1.f);
    setPosition(Engine::Core::Maths::Vec3::zero);
    setVelocity(Engine::Core::Maths::Vec3::zero);
    setLooping(false);

    loadWav(path);

    alSourcei(_source, AL_BUFFER, _buffer);
}

Sound::Sound(const char *path, float pitch, float gain, Engine::Core::Maths::Vec3 &position, Engine::Core::Maths::Vec3 &velocity, bool looping)
{
    alGenSources((ALuint)1, &_source);

    setPitch(pitch);
    setGain(gain);
    setPosition(position);
    setVelocity(velocity);
    setLooping(looping);

    loadWav(path);

    alSourcei(_source, AL_BUFFER, _buffer);
}

Sound::~Sound()
{
    alDeleteSources(1, &_source);
    alDeleteBuffers(1, &_buffer);
}
