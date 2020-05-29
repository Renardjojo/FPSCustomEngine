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
                : Model (refGameObject, arg)
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

            #pragma endregion //!attribut
    
            #pragma region methods

            void sendToShaderModelMatrix () const noexcept final
            {
                Engine::Core::Maths::Mat4 modelMat;
                const Engine::Core::Maths::Mat4& modelMatCam =  Camera::getCamUse()->getModelMatrix();

                /*Rotation*/
                /*Right*/
                modelMat[0][0] = modelMatCam[0][0];
                modelMat[0][1] = modelMatCam[0][1];
                modelMat[0][2] = modelMatCam[0][2];

                /*Up*/
                modelMat[1][0] = modelMatCam[1][0];
                modelMat[1][1] = modelMatCam[1][1];
                modelMat[1][2] = modelMatCam[1][2];

                /*Forward*/
                modelMat[2][0] = modelMatCam[2][0];
                modelMat[2][1] = modelMatCam[2][1];
                modelMat[2][2] = modelMatCam[2][2];

                /*Translation*/
                modelMat[3][0] = _gameObject.getGlobalPosition().x;
                modelMat[3][1] = _gameObject.getGlobalPosition().y;
                modelMat[3][2] = _gameObject.getGlobalPosition().z;

                /*Scale*/
                modelMat *= Engine::Core::Maths::Mat4::createScaleMatrix(_gameObject.getScale());
                pShader_->setMat4("model", &modelMat.mat[0]);
            }

            #pragma endregion //!methods
    
        private:
    };
}

#endif // _GE_BILLBOARD_H