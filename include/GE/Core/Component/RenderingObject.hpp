//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-18 - 08 h 10

#ifndef _RENDERING_OBJECT_H
#define _RENDERING_OBJECT_H

#include "GE/Ressources/Component.hpp"

namespace Engine::Core::Component
    {
    class RenderingObject : public Engine::Ressources::Component
    {
        private:

        protected:

        #pragma region attribut
        #pragma endregion //!attribut

        #pragma region static attribut
        #pragma endregion //! static attribut

        #pragma region methods
        #pragma endregion //!methods

        public:

        #pragma region constructor/destructor

        RenderingObject ()					                    = delete;
        RenderingObject (const RenderingObject& other)			= delete;
        RenderingObject (RenderingObject&& other)				= delete;
        RenderingObject& operator=(RenderingObject const&)		= delete;
        RenderingObject& operator=(RenderingObject &&)			= delete;

        RenderingObject(Engine::Ressources::GameObject &refGameObject);
        RenderingObject(const RenderingObject &other);
        RenderingObject(RenderingObject &&other);
        virtual ~RenderingObject();

        #pragma endregion //!constructor/destructor

        #pragma region methods

        virtual void draw () const noexcept = 0;

        #pragma endregion //!methods

        #pragma region accessor
        #pragma endregion //!accessor

        #pragma region mutator
        #pragma endregion //!mutator

        #pragma region operator
        #pragma endregion //!operator

        #pragma region convertor
        #pragma endregion //!convertor
    };

} /*namespace Engine::Core::Component */

#endif //_RENDERING_OBJECT_H
