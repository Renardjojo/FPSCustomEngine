//Project : Engine
//Editing by Six Jonathan
//Date : 14/01/2020 - 18 h 04

#ifndef _GE_LIGHT_H
#define _GE_LIGHT_H

#include <cmath>
#include <vector>

#include "glad/glad.h"
#include "GE/Core/Maths/vec.hpp"
#include "GE/LowRenderer/entity.hpp"
#include "GE/LowRenderer/camera.hpp"
#include "GE/Ressources/shader_type.hpp"

namespace Engine::LowRenderer
{
    typedef struct S_LightCreateArg
    {
        const Engine::Core::Maths::Vec3&                 pos;
        const Engine::Ressources::AmbiantComponent&      ambient; 
        const Engine::Ressources::DiffuseComponent&      diffuse;
        const Engine::Ressources::SpecularComponent&     specular;
        const char*                                      name;

    } LightCreateArg;

    class Light //TODO: Can be more optimize change information only when light is update
        : public Entity
    {
        public:
    
            #pragma region constructor/destructor
    
            Light ()						= default;

            Light (const LightCreateArg& arg);

            Light ( const Engine::Core::Maths::Vec3&                pos,
                    const Engine::Ressources::AmbiantComponent&     ambient, 
                    const Engine::Ressources::DiffuseComponent&     diffuse, 
                    const Engine::Ressources::SpecularComponent&    specular,
                    const char* name);
            
            Light (const Light& other)		= default;
            Light (Light&& other)			= default;
            virtual ~Light ()				= default;
    
            #pragma endregion //!constructor/destructor
    
            #pragma region methods
   
            virtual void addToLightToUseBuffer(std::vector<Engine::Ressources::light>& lb) noexcept
            {
                lb.push_back({  ambientComp_, 
                                diffuseComp_, 
                                specularComp_, 
                                position_, 0.f,
                                0.f, 0.f, 0.f, 0.f,
                                {0.f, 0.f, 0.f}, 0.f});
            }

            #pragma endregion //!methods
    
            #pragma region static methods

            #pragma endregion //!static methods            
    
            #pragma region accessor

            virtual const Engine::Core::Maths::Vec3&                        getPos()                   const noexcept { return position_; }
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

            static void resetLight()
            {
                lightsToUse.clear();
            }
    
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
