//Project : Engine
//Editing by Six Jonathan
//Date : 23/01/2020 - 12 h 57

#ifndef _GE_MUSIC_H
#define _GE_MUSIC_H

#include <SDL2/SDL_mixer.h>
#include "GE/Ressources/sound.hpp"

namespace Engine::Ressources
{
//function based on SDL2_mixer library. See documentation on : http://sdl.beuc.net/sdl.wiki/SDL_mixer
	struct MusicCreateArg
	{
		const char* MusicPath;
		unsigned int volume = DEFAULT_VOLUME;		

	};

	class Music
		: public Sound
	{
		public:

			#pragma region constructor/destructor

			Music ()						= default;
			/**
			 * @brief Construct a new Music object.
			 * Load all Music of struct S_Musics and inti SDL audio component. 
			 * Only support for Music in WAV, AIFF, RIFF and VOC format. 
			 * Can be upgrad to add OGG, MP3, FLAC and MOD format.

			* 
			* @param MusicPath 
			* @param frequency 
			* @param format 
			* @param channels 
			* @param chunksize 
			* @param volume 
			*/
			Music (const char* MusicPath, unsigned int volume = DEFAULT_VOLUME);
			Music (const MusicCreateArg& arg);
			
			Music (const Music& other)		= default;
			Music (Music&& other)			= default;

			/**
			 * @brief Destroy the Music object and close SDL audio componente if it's the last Music
			 * 
			 */
			virtual ~Music ();

			#pragma endregion //!constructor/destructor

			#pragma region methods

			/**
			 * @brief Play Music : 0 for just 1 loop. -1 for infinit loop
			 * 
			 * @param loop 
			 */
			virtual void play			(int loop = 0) noexcept override;

			/**
			 * @brief halt the Music in it current channel
			 * 
			 */
			virtual void halt			() noexcept override;

			/**
			 * @brief Set Music volume : min = 0 | max = MIX_MAX_VOLUME = 128
			 * 
			 * @param volume 
			 */
			virtual void setVolume	(unsigned int volume) noexcept override;

			/**
			 * @brief pause the Music
			 * 
			 */
			virtual void pause				() noexcept override;


			/**
			 * @brief resume the Music
			 * 
			 */
			virtual void resume			() noexcept override;

			#pragma endregion //!methods

			#pragma region static methods

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

			Mix_Music*		music_;

			#pragma endregion //!attribut

			#pragma region static attribut
			#pragma endregion //! static attribut

			#pragma region methods
			#pragma endregion //!methods

		private:
	};
} //namespace Engine::Ressources

#endif //_GE_MUSIC_H
