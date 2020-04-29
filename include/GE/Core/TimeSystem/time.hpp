//Project : Engine
//Editing by Six Jonathan
//Date : 27/01/2020 - 17 h 02

#ifndef _GE_TIME_MANAGER_H
#define _GE_TIME_MANAGER_H

#include <algorithm>

namespace Engine::Core::TimeSystem
{

	#define DEFAULT_FPS_MAX 0xffffffff //60

	class ChronoDt
	{
		public:
			ChronoDt () = default;
			ChronoDt (float delay);
			ChronoDt (const ChronoDt& other) = default;
			virtual ~ChronoDt ();

		/*----------*/
		/* methode  */
		/*----------*/

		//update chrono statu with delta time. If delta time == 0. So Chrono is paused
		void update(float delta_time);

		//reset chrono with same delay
		void reset();

		/*----------*/
		/* accessor */
		/*----------*/

		//return if time delay is pass
		bool isEnd()	const;

			/*----------*/
			/* mutator  */
		/*----------*/

			/*----------*/
			/* operator */
		/*----------*/

			/*----------*/
			/* convertor*/ 
		/*----------*/

		//public variable (get and set with no effect for class)
		float delay_;
		float time_;

		protected:

		bool isFinish_;			

		private:
	};

	typedef struct S_FPS
	{
		ChronoDt		chrono {500};
		unsigned int 	FPSmax;

	} FPS;

	class TimeSystem
	{
		public:

		#pragma region constructor/destructor

		TimeSystem ()									= delete;
		TimeSystem (const TimeSystem& other)			= delete;
		TimeSystem (TimeSystem&& other)					= delete;
		virtual ~TimeSystem ()							= delete;
		TimeSystem& operator=(TimeSystem const&)		= delete;
		TimeSystem& operator=(TimeSystem &&)			= delete;

		#pragma endregion //!constructor/destructor

		#pragma region methods

		static void update();

		#pragma endregion //!methods

		#pragma region accessor

		static float getDeltaTime()				{ return deltaTime_;}
		static float getUnscaledDetlaTime()		{ return unscaledDetlaTime_;}
		static float getTimeScale()				{ return timeScale_;}

		#pragma endregion //!accessor

		#pragma region mutator

		static float setTimeScale(float timeScale)	{ return timeScale_ = timeScale; }

		#pragma endregion //!mutator

		#pragma region operator
		#pragma endregion //!operator

		#pragma region convertor
		#pragma endregion //!convertor

		protected:

		#pragma region attribut

		static float deltaTime_;
		static float unscaledDetlaTime_;
		static float timeScale_;

		static FPS	FPS_;

		#pragma endregion //!attribut

		#pragma region static attribut
		#pragma endregion //! static attribut

		#pragma region methods

		#pragma endregion //!methods

		private:

		static float timeExFrame_;
	};

}//namespace Engine::Core::TimeSystem

#endif // _GE_TIME_MANAGER_H
