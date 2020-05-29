#ifndef __SOUND_HPP__
#define __SOUND_HPP__

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include "GE/Core/Maths/vec.hpp"

namespace Engine::Ressources
{
    class Sound
    {
    private:
        ALuint _source;
        ALuint _buffer;

        ALenum _format;
        ALsizei _size;
        ALfloat _frequency;

        bool loadWav(const char *path);

    public:
        Sound(const char *path);
        Sound(const char *path, float pitch, float gain, Engine::Core::Maths::Vec3 &position, Engine::Core::Maths::Vec3 &velocity, bool looping);
        ~Sound();

        bool setPitch(float p);
        bool setGain(float g);
        bool setPosition(Engine::Core::Maths::Vec3 &p);
        bool setVelocity(Engine::Core::Maths::Vec3 &v);
        bool setLooping(bool b);

        ALuint getSource();
    };

    inline bool Sound::loadWav(const char *path)
    {
        ALvoid *data = alutLoadMemoryFromFile(path,
                                              &_format,
                                              &_size,
                                              &_frequency);

        alBufferData(_buffer, _format, data, _size, _frequency);
        return true; // todo check errors plz
    }

    inline bool Sound::setPitch(float p)
    {
        alSourcef(_source, AL_PITCH, p);
        return true; //todo : test actual errors plz
    }
    inline bool Sound::setGain(float g)
    {
        alSourcef(_source, AL_GAIN, g);
        return true; //todo : test actual errors plz
    }
    inline bool Sound::setPosition(Engine::Core::Maths::Vec3 &p)
    {
        alSource3f(_source, AL_POSITION, p.x, p.y, p.z);
        return true; //todo : test actual errors plz
    }
    inline bool Sound::setVelocity(Engine::Core::Maths::Vec3 &v)
    {
        alSource3f(_source, AL_VELOCITY, v.x, v.y, v.z);
        return true; //todo : test actual errors plz
    }
    inline bool Sound::setLooping(bool b)
    {
        alSourcei(_source, AL_LOOPING, b);
        return true; //todo : test actual errors plz
    }

    inline ALuint Sound::getSource()
    {
        return _source;
    }

} // namespace Engine::Ressources

#endif // __SOUND_HPP__