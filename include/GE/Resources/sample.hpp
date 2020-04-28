//Project : Engine
//Editing by Six Jonathan
//Date : 23/01/2020 - 09 h 04

#ifndef _GE_SAMPLE_H
#define _GE_SAMPLE_H

#include <SDL2/SDL_mixer.h>
#include "sound.hpp"

//function based on SDL2_mixer library. See documentation on : http://sdl.beuc.net/sdl.wiki/SDL_mixer
namespace Engine::Resources
{
	struct SampleCreateArg
	{
		const char* SamplePath;
		unsigned int volume = DEFAULT_VOLUME;		

	};

	class Sample
		: public Sound
	{
		public:

			#pragma region constructor/destructor

			Sample ()						= default;
			/**
			 * @brief Construct a new Sample object.
			 * Load all sample of struct S_Samples and inti SDL audio component. 
			 * Only support for Sample in WAV, AIFF, RIFF and VOC format. 
			 * Can be upgrad to add OGG, MP3, FLAC and MOD format.

			* 
			* @param SamplePath 
			* @param frequency 
			* @param format 
			* @param channels 
			* @param chunksize 
			* @param volume 
			*/
			Sample (const char* SamplePath, unsigned int volume = DEFAULT_VOLUME);
			Sample (const SampleCreateArg& arg);
			
			Sample (const Sample& other)		= default;
			Sample (Sample&& other)				= default;

			/**
			 * @brief Destroy the Sample object and close SDL audio componente if it's the last Sample
			 * 
			 */
			virtual ~Sample ();

			#pragma endregion //!constructor/destructor

			#pragma region methods

			/**
			 * @brief Play sample : 0 for just 1 loop. -1 for infinit loop
			 * 
			 * @param loop 
			 */
			virtual void play			(int loop = 0) noexcept override;

			/**
			 * @brief halt the Sample in it current channel
			 * 
			 */
			virtual void halt			() noexcept override;

			/**
			 * @brief Set sample volume : min = 0 | max = MIX_MAX_VOLUME = 128
			 * 
			 * @param volume 
			 */
			virtual void setVolume	(unsigned int volume) noexcept override;

			/**
			 * @brief pause the Sample
			 * 
			 */
			virtual void pause				() noexcept override;


			/**
			 * @brief resume the Sample
			 * 
			 */
			virtual void resume			() noexcept override;

			#pragma endregion //!methods

			#pragma region static methods

			/**
			 * @brief set general Sample volume : min = 0 | max = MIX_MAX_VOLUME = 128
			 * 
			 * @param volume 
			 */
			static
			void setGeneralSampleVolume	(unsigned int volume) noexcept;

			/**
			 * @brief pause all channel in activity
			 * 
			 */
			static
			void pauseAll			() noexcept;

			/**
			 * @brief resume all channel in activity
			 * 
			 */
			static
			void resumeAll			() noexcept;

			/**
			 * @brief halt all the channel
			 * 
			 */
			void haltAll			() noexcept;

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

			Mix_Chunk*		sample_;
			int 			channelUse;

			#pragma endregion //!attribut

			#pragma region static attribut
			
			static unsigned int generalSampleVolume;

			#pragma endregion //! static attribut

			#pragma region methods
			#pragma endregion //!methods

		private:

	};
} // namespace Engine::Resources

#endif //_GE_SAMPLE_H
