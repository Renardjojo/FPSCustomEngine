//Project : Engine
//Editing by Six Jonathan
//Date : 14/01/2020 - 18 h 04

#ifndef _GE_LIGHT_H
#define _GE_LIGHT_H

#include <vector>

#include "GE/Core/Maths/vec.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Ressources/Component.hpp"
#include "GE/LowRenderer/camera.hpp"
#include "GE/Ressources/shader_type.hpp"

namespace Engine::LowRenderer
{
    typedef struct S_LightCreateArg
    {
        const Engine::Ressources::AmbiantComponent&      ambient; 
        const Engine::Ressources::DiffuseComponent&      diffuse;
        const Engine::Ressources::SpecularComponent&     specular;
        bool isEnable                                   {true};

    } LightCreateArg;

    class Light //TODO: Can be more optimize change information only when light is update
        : public Engine::Ressources::Component
    {
        public:
    
            #pragma region constructor/destructor

            Light (Engine::Ressources::GameObject &refGameObject, const LightCreateArg& arg);

            Light ( Engine::Ressources::GameObject &refGameObject,
                    const Engine::Ressources::AmbiantComponent&     ambient, 
                    const Engine::Ressources::DiffuseComponent&     diffuse, 
                    const Engine::Ressources::SpecularComponent&    specular,
                    bool isEnable = true);
            
            Light (const Light& other)		= delete;
            Light (Light&& other)			= delete;
            virtual ~Light ();

            Light()                               = delete;
            Light &operator=(Light const &other)  = delete;
            Light &operator=(Light &&other)       = delete;
    
            #pragma endregion //!constructor/destructor
    
            #pragma region methods
   
            virtual void addToLightToUseBuffer(std::vector<Engine::Ressources::light>& lb) noexcept
            {
                lb.push_back({  ambientComp_, 
                                diffuseComp_, 
                                specularComp_, 
                                getGameObject().getGlobalPosition(), 0.f,
                                0.f, 0.f, 0.f, 0.f,
                                {0.f, 0.f, 0.f}, 0.f});
            }

            #pragma endregion //!methods
    
            #pragma region static methods

            #pragma endregion //!static methods            
    
            #pragma region accessor

            virtual const Engine::Ressources::AmbiantComponent&             getAmbient()               const noexcept { return ambientComp_; }
            virtual const Engine::Ressources::DiffuseComponent&             getDiffuse()               const noexcept { return diffuseComp_; }
            virtual const Engine::Ressources::SpecularComponent&            getSpecular()              const noexcept { return specularComp_; }
            virtual bool                                                    isEnable()                 const noexcept { return isEnable_; }

            /**
             * @brief Get the Lights To Use object converte in alignas struct to be load in block in shader
             * 
             * @return std::vector<Engine::LowRenderer::light> 
             */
            static
            std::vector<Engine::Ressources::light> getLightsToUseInAlignasStruct ();

            #pragma endregion //!accessor
    
            #pragma region mutator

            virtual void  setGlobalComponent(const Engine::Ressources::ColorRGBA& newComponent)  noexcept
            {
                ambientComp_.rgba = newComponent;
                diffuseComp_.rgba = newComponent;
                specularComp_.rgba = newComponent;
            }

            virtual void  setGlobalComponent(const Engine::Core::Maths::Vec4& newComponent)  noexcept
            {
                ambientComp_.rgbi = newComponent;
                diffuseComp_.rgbi = newComponent;
                specularComp_.rgbi = newComponent;
            }

            virtual void  setAmbient(const Engine::Ressources::AmbiantComponent& newAmbient)          noexcept { ambientComp_.rgba = newAmbient.rgba; }
            virtual void  setDiffuse(const Engine::Ressources::DiffuseComponent& newDiffuse)          noexcept { diffuseComp_.rgba = newDiffuse.rgba; }
            virtual void  setSpecular(const Engine::Ressources::SpecularComponent& newSpecular)         noexcept { specularComp_.rgba = newSpecular.rgba; }

            virtual void  setAmbient(const Engine::Core::Maths::Vec4& newAmbient)          noexcept { ambientComp_.rgbi = newAmbient; }
            virtual void  setDiffuse(const Engine::Core::Maths::Vec4& newDiffuse)          noexcept { diffuseComp_.rgbi = newDiffuse; }
            virtual void  setSpecular(const Engine::Core::Maths::Vec4& newSpecular)         noexcept { specularComp_.rgbi = newSpecular; }

            virtual void  enable (bool flag)               throw();

            #pragma endregion //!mutator
    

            static void resetLight()
            {
                lightsToUse.clear();
            }

#ifndef DNEDITOR
            virtual void serializeOnEditor () noexcept override;
#endif
    
        protected:
    
            #pragma region attribut

            Engine::Ressources::AmbiantComponent        ambientComp_;
            Engine::Ressources::DiffuseComponent        diffuseComp_;
            Engine::Ressources::SpecularComponent       specularComp_;

            bool                    isEnable_;

            #pragma endregion //!attribut
    
            static std::vector<Light*> lightsToUse;

    
        private:
    
    };
}

#endif //_GE_LIGHT_H
