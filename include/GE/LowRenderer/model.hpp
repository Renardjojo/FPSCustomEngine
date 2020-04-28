//Project : Engine
//Editing by Six Jonathan
//Date : 10/01/2020 - 14 h 05

#ifndef _GE_Model_H
#define _GE_Model_H

#include "iModel.hpp"
#include "vec.hpp"
#include "transform.hpp"
#include "material.hpp"
#include "texture.hpp"
#include "mesh.hpp"
#include "shader.hpp"

namespace Engine::LowRenderer
{
    typedef struct S_ModelCreateArg
    {
        Engine::Core::Maths::Vec3                       position, rotation, scale;
        Engine::Resources::Shader*                      pShader;
        std::vector<Engine::Resources::Material*>       pMaterials;
        Engine::Resources::Mesh*                        pMesh;
        const char*                                     name;
         
        bool                                            loadInGPU               = true;
        bool                                            enableBackFaceCulling   = true;
        bool                                            isOpaque                = true;

    } ModelCreateArg;

    class Model
        : public IModel
    {
        public:
    
            #pragma region constructor/destructor
    
            Model (const ModelCreateArg& arg);
            Model (const Model& other)        = default;
            Model (Model&& other)             = default;
            ~Model ();
    
            #pragma endregion //!constructor/destructor
    
            #pragma region methods

            /**
             * @brief Draw element only if it is load in GPU
             * 
             */
            void draw ()        const noexcept override;

            /**
             * @brief Load texture and Mesh from CPU to GPU. This operation can be slow.
             * 
             */            
            void loadInGPU()        noexcept final;
            void unloadFromGPU()    noexcept final;

            #pragma endregion //!methods
    
            #pragma region static methods

            //nullptr if don't use the shader else set pointor to shader to use
            void useSimpleShader(Engine::Resources::Shader* pShader);

            #pragma endregion //!static methods
    
            #pragma region accessor

            bool isOpaque () const noexcept { return isOpaque_; }

            #pragma endregion //!accessor
    
            #pragma region mutator

            #pragma endregion //!mutator
    
            #pragma region operator
            #pragma endregion //!operator
    
            #pragma region convertor
            #pragma endregion //!convertor
    
        protected:
    
            #pragma region attribut

            Engine::Resources::Shader*                  pShader_;
            std::vector<Engine::Resources::Material*>   pMaterial_; //contain the texture and material data
            std::vector<Engine::Resources::Material*>   pMaterialToUse_; //contain pointor to the material to use when model is display.
            Engine::Resources::Mesh*                    pMesh_;

            bool                                        enableBackFaceCulling_;
            bool                                        isOpaque_;
            
            #pragma endregion //!attribut
    
            #pragma region methods

            virtual 
            void sendToShaderModelMatrix () const noexcept;

            #pragma endregion //!methods
    
        private:

            /**
             * @brief fill pTextureToUse in function of 
             *  
             */
            void initTextureBufferWithMTLId ();
    };
}

#endif // _GE_Model_H