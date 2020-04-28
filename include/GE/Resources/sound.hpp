//Project : Engine
//Editing by Six Jonathan
//Date : 23/01/2020 - 11 h 44

#ifndef _GE_SOUND_H
#define _GE_SOUND_H

#include <SDL2/SDL_mixer.h>
#include "log.hpp"
#include "assert.hpp"

namespace Engine::Resources
{
	#define DEFAULT_VOLUME MIX_MAX_VOLUME / 2

    struct SoundSettingArg
    {
        int frequency 		= MIX_DEFAULT_FREQUENCY; 
        unsigned int format = MIX_DEFAULT_FORMAT; 
        int channels 		= MIX_DEFAULT_CHANNELS; 
        int chunksize 		= 1024;	
        unsigned int generalVolume 	= DEFAULT_VOLUME;
        int maskSoundSupport		= MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG;
    };

    class Sound
    {
        public:

            #pragma region constructor/destructor

            Sound ();
            Sound (const Sound& other);
            Sound (Sound&& other)			= default;
            virtual ~Sound ();

            #pragma endregion //!constructor/destructor

            #pragma region methods

            virtual void play			(int loop) noexcept             = 0;
            virtual void halt			() noexcept                     = 0;
            virtual void setVolume	    (unsigned int volume) noexcept  = 0;
            virtual void pause			() noexcept                     = 0;
            virtual void resume			() noexcept                     = 0;

            #pragma endregion //!methods

            #pragma region static methods

			/**
			 * @brief set Sound setting befor initialisation of the first Sound
			 * 
			 * Flag support for maskSoundSupport : [type1]|[type2]|....
			 * MIX_INIT_FLAG
			 * MIX_INIT_MOD
			 * MIX_INIT_MP3
			 * MIX_INIT_OGG
			 */
			static
			void setSettingSoundInit				(const SoundSettingArg& arg) noexcept;

			/**
			 * @brief set general sound volume : min = 0 | max = MIX_MAX_VOLUME = 128
			 * 
			 * @param volume 
			 */
			static
			void setGeneralVolume	(unsigned int volume) noexcept;

            #pragma endregion //!static methods

            #pragma region accessor
            #pragma endregion //!accessor

            #pragma region mutator
            #pragma endregion //!mutator

            #pragma region operator
            #pragma endregion //!operator

            #pragma region convertor
            #pragma endregion //!convertor

        protected:

            #pragma region attribut
            #pragma endregion //!attribut

            #pragma region static attribut

			static bool SDLIsInit;
			static int numberSound;
			static int frequency; 
			static unsigned int format;
			static int channels;
			static int chunksize;
			static int maskSoundSupport;
            static unsigned int generalVolume;

            #pragma endregion //! static attribut

            #pragma region methods
            
            void initSDLSoundComponent 	() noexcept;
            void releaseSDLSoundComponent 	() noexcept;

            #pragma endregion //!methods

        private:

    };
}; //namespace Engine::Resources

#endif // _GE_SOUND_H