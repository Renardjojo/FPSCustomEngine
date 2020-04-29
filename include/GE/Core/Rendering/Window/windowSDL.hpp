//Project : Engine
//Editing by Six Jonathan
//Date : 09/01/2020 - 09 h 17

#include <SDL2/SDL_video.h>
#include "GE/Core/Rendering/Window/iWindow.hpp"

#ifndef _GE_WINDOW_SDL_H
#define _GE_WINDOW_SDL_H

namespace Engine::Core::Renderer
{
    class WindowSDL
        : public IWindow
    {
        public:

            #pragma region constructor/destructor

            ~       WindowSDL ();
            void    initialize (const WindowCreateArg& arg) noexcept final;
            void    release   ()                            noexcept final;   

            #pragma endregion //!constructor/destructor

            #pragma region methods
            #pragma endregion //!methods

            #pragma region static methods
            #pragma endregion //!static methods

            #pragma region accessor

            /**
             * @brief return SDL_Window
             * 
             * @return void* 
             */
            void* get () const noexcept final { return win; }

            unsigned int getId () const noexcept final { return SDL_GetWindowID(win); }

            Engine::Resources::Size    getSize () const noexcept final 
            { 
                Engine::Resources::Size rst;
                SDL_GetWindowSize(win, &rst.width, &rst.heigth);
                return rst;
            }

            #pragma endregion //!accessor

            #pragma region mutator
            #pragma endregion //!mutator

            #pragma region operator
            #pragma endregion //!operator

            #pragma region convertor
            #pragma endregion //!convertor

        public:

            #pragma region attribut

            SDL_Window* win;

            #pragma endregion //!attribut

            #pragma region methods
            #pragma endregion //!methods

        private:

    };

} //namespace Engine::Core::Renderer

#endif // _GE_WINDOW_SDL_H