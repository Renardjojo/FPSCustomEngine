//Project : Engine
//Editing by Six Jonathan
//Date : 09/01/2020 - 09 h 26

#ifndef _INTERFACE_WINDOW_H
#define _INTERFACE_WINDOW_H

#include "GE/Ressources/type.hpp"

namespace Engine::Core::Renderer
{
    typedef struct S_WindowCreateArg
    {
        const char*     name;
        unsigned int    height;
        unsigned int    width;

    } WindowCreateArg;

    class IWindow
    {
        public:

            #pragma region constructor/destructor
            
            virtual ~       IWindow   () = default;
            virtual void    initialize (const WindowCreateArg& arg) = 0;
            virtual void    release   () = 0;

            #pragma endregion //!constructor/destructor

            #pragma region methods
            #pragma endregion //!methods

            #pragma region static methods
            #pragma endregion //!static methods

            #pragma region accessor
            
            virtual void*                       get     () const noexcept = 0;
            virtual unsigned int                getId   () const noexcept = 0;
            virtual Engine::Ressources::Size     getSize () const noexcept = 0;

            #pragma endregion //!accessor

            #pragma region mutator
            #pragma endregion //!mutator

            #pragma region operator
            #pragma endregion //!operator

            #pragma region convertor
            #pragma endregion //!convertor

        protected:

        private:

    };
} //namespace Engine::Core::Renderer

#endif // _INTERFACE_WINDOW_H