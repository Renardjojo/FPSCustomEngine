//Project : Engine
//Editing by Six Jonathan
//Date : 09/01/2020 - 10 h 14

#include "GE/Core/Rendering/Renderer/iRenderer.hpp"
#include "glad/glad.h"

#include <SDL2/SDL_video.h>

#ifndef DNEDITOR
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#endif


#ifndef _RENDERER_SDL_OPENGL_3_3_H
#define _RENDERER_SDL_OPENGL_3_3_H

namespace Engine::Core::Renderer
{
    class RendererSDLOpGl33
        : public IRenderer
    {
        public:

            #pragma region constructor/destructor

            virtual ~       RendererSDLOpGl33 ();
            virtual void    initialize  (const WindowCreateArg& arg);
            virtual void    release     ();

            #pragma endregion //!constructor/destructor

            #pragma region methods
        
            void  clear  () noexcept final { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); }
            void  draw   () noexcept final
            { 
#ifndef DNEDITOR
                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
                SDL_GL_SwapWindow((SDL_Window*)pWin->get());

#ifndef DNEDITOR
                /*Prepar the next imGui frame*/
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplSDL2_NewFrame((SDL_Window*)pWin->get());
                ImGui::NewFrame();
#endif
            }

            #pragma endregion //!methods

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

            SDL_GLContext openGlContext_;

            #pragma endregion //! static attribut

            #pragma region methods
            #pragma endregion //!methods

        private:

    };
} //namespace Engine::Core::Renderer

#endif // _RENDERER_SDL_OPENGL_3_3_H