//Project : Engine
//Editing by Six Jonathan
//Date : 10/01/2020 - 14 h 05

#ifndef _GE_Model_H
#define _GE_Model_H

#include "GE/LowRenderer/iModel.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Physics/transform.hpp"
#include "GE/Ressources/material.hpp"
#include "GE/Ressources/texture.hpp"
#include "GE/Ressources/mesh.hpp"
#include "GE/Ressources/shader.hpp"

namespace Engine::LowRenderer
{
    typedef struct S_ModelCreateArg
    {
        Engine::Ressources::Shader*                     pShader                = nullptr;
        std::vector<Engine::Ressources::Material*>      pMaterials;
        Engine::Ressources::Mesh*                       pMesh                  = nullptr;

        std::string                                     shaderName;
        std::vector<std::string>                        materialName; 
        std::string                                     meshName; 
         
        bool                                            loadInGPU               = true;
        bool                                            enableBackFaceCulling   = true;
        bool                                            isOpaque                = true;

    } ModelCreateArg;

    class Model
        : public IModel
    {
        public:
    
            #pragma region constructor/destructor
    
            Model(Engine::Ressources::GameObject &refGameObject, const ModelCreateArg& arg);

            Model(const Model &other)                             = default;
            Model(Model &&other)                                  = default;
            virtual ~Model();

            Model()                                               = delete;
            Model &operator=(Model const &other)                  = delete;
            Model &operator=(Model &&other)                       = delete;

    
            #pragma endregion //!constructor/destructors
    
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
            void loadInGPU()        noexcept;
            void unloadFromGPU()    noexcept;

            #pragma endregion //!methods
    
            #pragma region static methods

            //nullptr if don't use the shader else set pointor to shader to use
            void useSimpleShader(Engine::Ressources::Shader* pShader);

            #pragma endregion //!static methods
    
            #pragma region accessor

            bool isOpaque () const noexcept { return isOpaque_; }

            std::string getShaderName() { return shaderName_; }
            std::vector<std::string>& getMaterialName() { return materialName_; }
            std::string getMeshName() { return meshName_; }

            #pragma endregion //!accessor
    
    
        protected:
    
            #pragma region attribut

            Engine::Ressources::Shader*                  pShader_;
            std::vector<Engine::Ressources::Material*>   pMaterial_; //contain the texture and material data
            std::vector<Engine::Ressources::Material*>   pMaterialToUse_; //contain pointor to the material to use when model is display.
            Engine::Ressources::Mesh*                    pMesh_;

            std::string                                  shaderName_;
            std::vector<std::string>                     materialName_; 
            std::string                                  meshName_; 

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