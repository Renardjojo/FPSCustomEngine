//Project : Engine
//Editing by Six Jonathan
//Date : 27/01/2020 - 17 h 02

#ifndef _GE_TIME_MANAGER_H
#define _GE_TIME_MANAGER_H

namespace Engine::Core::TimeSystem
{

	#define DEFAULT_FPS_MAX 60

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

	} T_FPS;

	class TimeManager
	{
		public:
			TimeManager (unsigned int FPSmax = DEFAULT_FPS_MAX);
			TimeManager (const TimeManager& other)			= default;
			~TimeManager () 								= default;

			/*----------*/
			/* methode  */
			/*----------*/

			//update current time and add delay to correspond to the good FPS
			void update();

			bool secondIsPassing () { return FPS_.chrono.time_ == 0.f; }

			/*----------*/
			/* accessor */
			/*----------*/

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
			unsigned int 	time_;
			float			dtf_; //delta frame frame


		protected:

			T_FPS	FPS_;

		private:
	};
}//namespace Engine::Core::TimeSystem

#endif // _GE_TIME_MANAGER_H
