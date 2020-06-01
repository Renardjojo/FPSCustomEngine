#ifndef __SOUNDPLAYER_HPP__
#define __SOUNDPLAYER_HPP__

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include <vector>
#include <string>


#include "GE/Core/Debug/log.hpp"
#include "GE/Core/Debug/assert.hpp"

namespace Engine::Ressources
{
    class Sound;

    class SoundPlayer
    {
    private:
        static bool _initialized;
        static ALCdevice *_device;
        static ALCcontext *_context;
        static std::vector<std::string> _devices;

    public:
        SoundPlayer()=delete;
        ~SoundPlayer()=delete;
        SoundPlayer(const SoundPlayer &other) = delete;
        SoundPlayer(SoundPlayer &&other) = delete;

        static bool initialize();
        static void destroy();
        static void listDevices();

        static void play(Sound& sound);
        static void stop(Sound& sound);
        static void pause(Sound& sound);
        static void rewind(Sound& sound);
    };
}; //namespace Engine::Ressources

#endif // __SOUNDPLAYER_HPP__