//Project : Engine
//Editing by Six Jonathan
//Date : 10/01/2020 - 14 h 05

#ifndef _GE_BILLBOARD_H
#define _GE_BILLBOARD_H

#include "model.hpp"
#include "mat.hpp"
#include "vec.hpp"
#include "camera.hpp"
#include "assert.hpp"

namespace Engine::LowRenderer
{
    class BillBoard
        : public Model
    {
        public:
    
            #pragma region constructor/destructor
    
            BillBoard (const ModelCreateArg& arg)
                : Model (arg),
                  posParent_()
            {}

            BillBoard (const BillBoard& other)        = default;
            BillBoard (BillBoard&& other)             = default;
            BillBoard ();
    
            #pragma endregion //!constructor/destructor
    
            #pragma region methods

            /**
             * @brief Update Mesh matrix 
             * 
             */
            virtual void update () noexcept
            {
                isDirty_ = false;
            }

            /**
             * @brief Update transform if it depend to parent Mesh view (use in inherance in scene graph)
             * 
             * @param parentMeshMatrix : Mesh view matrix of parent
             */
            virtual void update (const Engine::Core::Maths::Mat4& parentMeshMatrix) noexcept
            {
                posParent_.x = parentMeshMatrix[3][0];
                posParent_.y = parentMeshMatrix[3][1];
                posParent_.z = parentMeshMatrix[3][2];
                isDirty_ = false;
            }

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
                Engine::Core::Maths::Mat4::createTranslationMatrix(position_ + posParent_) * 
                Engine::Core::Maths::Mat4(Engine::Core::Maths::Mat3::createLookAtView(position_ + posParent_, Camera::getCamUse()->getPosition(), {0.f, 1.f, 0.f})) *
                Engine::Core::Maths::Mat4::createXRotationMatrix(M_PI_2) * 
                Engine::Core::Maths::Mat4::createScaleMatrix(scale_);

                pShader_->setMat4("model", &modelMat.mat[0]);
            }



            #pragma endregion //!methods
    
        private:
    };
}

#endif // _GE_BILLBOARD_H