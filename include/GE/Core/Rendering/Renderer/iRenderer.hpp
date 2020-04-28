//Project : Engine
//Editing by Six Jonathan
//Date : 09/01/2020 - 09 h 11

#include <memory>
#include "iWindow.hpp"

#ifndef _GE_IRENDERER_H
#define _GE_IRENDERER_H

namespace Engine::Core::Renderer
{
    class IRenderer
    {
        public:

            #pragma region constructor/destructor

            virtual ~       IRenderer   () = default;
            virtual void    initialize  (const WindowCreateArg& arg) = 0;
            virtual void    release     () = 0;

            #pragma endregion //!constructor/destructor

            #pragma region methods
            #pragma endregion //!methods

            #pragma region static methods
            #pragma endregion //!static methods

            #pragma region accessor

            const IWindow* getWin () { return pWin.get(); }
            virtual void   draw  () noexcept = 0;
            virtual void   clear () noexcept = 0;
            
            #pragma endregion //!accessor

            #pragma region mutator
            #pragma endregion //!mutator

            #pragma region operator
            #pragma endregion //!operator

            #pragma region convertor
            #pragma endregion //!convertor

        protected:

            #pragma region attribut

            std::unique_ptr<IWindow>    pWin;

            #pragma endregion //!attribut

        private:

    };
} //namespace Engine::Core::Renderer


#endif // _GE_IRENDERER_H