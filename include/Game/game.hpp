//Project : Engine
//Editing by Six Jonathan
//Date : 08/01/2020 - 18 h 29

#ifndef _GAME_H
#define _GAME_H

#include "GE/GE.hpp"

namespace Game
{
    class Game
    {
        public:

            #pragma region constructor/destructor

            Game (Engine::GE& gameEngine);

            Game ()                     = delete;
            Game (const Game& other)    = default;
            ~Game ()                    = default;

            #pragma endregion //!constructor/destructor

            #pragma region methods

            //main loop
            void run ();

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

            Engine::GE&     gameEngine_;

            #pragma endregion //!attribut

            #pragma region static attribut

            #pragma endregion //! static attribut

            #pragma region methods
            #pragma endregion //!methods

        private:

    };
} //namespace Game

#endif // _GAME_H