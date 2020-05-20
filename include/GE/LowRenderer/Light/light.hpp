//Project : Engine
//Editing by Six Jonathan
//Date : 14/01/2020 - 18 h 04

#ifndef _GE_LIGHT_H
#define _GE_LIGHT_H

#include <cmath>
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
                    const Engine::Ressources::SpecularComponent&    specular);
            
            Light (const Light& other)		= delete;
            Light (Light&& other)			= delete;
            virtual ~Light ()				= default;

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

            virtual void                                enable (bool flag)               throw();

            #pragma endregion //!mutator
    
            #pragma region operator
            #pragma endregion //!operator
    
            #pragma region convertor
            #pragma endregion //!convertor
    
        protected:
    
            #pragma region attribut

            Engine::Ressources::AmbiantComponent        ambientComp_;
            Engine::Ressources::DiffuseComponent        diffuseComp_;
            Engine::Ressources::SpecularComponent       specularComp_;

            bool                    isEnable_;

            #pragma endregion //!attribut
    
            static std::vector<Light*> lightsToUse;

            #pragma region static attribut
            
            #pragma endregion //! static attribut
    
            #pragma region methods
            #pragma endregion //!methods
    
        private:
    
    };
}

#endif //_GE_LIGHT_H
