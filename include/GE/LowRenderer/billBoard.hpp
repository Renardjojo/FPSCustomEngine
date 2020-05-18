//Project : Engine
//Editing by Six Jonathan
//Date : 10/01/2020 - 14 h 05

#ifndef _GE_BILLBOARD_H
#define _GE_BILLBOARD_H

#include "GE/LowRenderer/model.hpp"
#include "GE/Core/Maths/mat.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/LowRenderer/camera.hpp"
#include "GE/Core/Debug/assert.hpp"

namespace Engine::LowRenderer
{
    class BillBoard
        : public Model
    {
        public:
    
            #pragma region constructor/destructor
    
            BillBoard (Engine::Ressources::GameObject &refGameObject, const ModelCreateArg& arg)
                : Model (refGameObject, arg),
                  posParent_()
            {}

            BillBoard (const BillBoard& other)        = default;
            BillBoard (BillBoard&& other)             = default;
            BillBoard ();
    
            #pragma endregion //!constructor/destructor
    
            #pragma region methods

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

            Engine::Core::Maths::Vec3 posParent_;

            #pragma endregion //!attribut
    
            #pragma region methods

            void sendToShaderModelMatrix () const noexcept final
            {
                Engine::Core::Maths::Mat4 modelMat =
                Engine::Core::Maths::Mat4::createTranslationMatrix(gameObject.getPosition()) * 
                Engine::Core::Maths::Mat4(Engine::Core::Maths::Mat3::createLookAtView(gameObject.getPosition(), Camera::getCamUse()->getPosition(), {0.f, 1.f, 0.f})) *
                Engine::Core::Maths::Mat4::createXRotationMatrix(M_PI_2) * 
                Engine::Core::Maths::Mat4::createScaleMatrix(gameObject.getScale());

                pShader_->setMat4("model", &modelMat.mat[0]);
            }

            #pragma endregion //!methods
    
        private:
    };
}

#endif // _GE_BILLBOARD_H