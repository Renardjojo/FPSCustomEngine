//Project : Engine
//Editing by Six Jonathan
//Date : 14/01/2020 - 18 h 04

#ifndef _GE_LIGHT_H
#define _GE_LIGHT_H

#include <cmath>
#include <vector>

#include "glad.h"
#include "vec.hpp"
#include "entity.hpp"
#include "camera.hpp"
#include "shader_type.hpp"

namespace Engine::LowRenderer
{
    typedef struct S_LightCreateArg
    {
        const Engine::Core::Maths::Vec3&                 pos;
        const Engine::Resources::AmbiantComponent&      ambient; 
        const Engine::Resources::DiffuseComponent&      diffuse;
        const Engine::Resources::SpecularComponent&     specular;
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
                    const Engine::Resources::AmbiantComponent&     ambient, 
                    const Engine::Resources::DiffuseComponent&     diffuse, 
                    const Engine::Resources::SpecularComponent&    specular,
                    const char* name);
            
            Light (const Light& other)		= default;
            Light (Light&& other)			= default;
            virtual ~Light ()				= default;
    
            #pragma endregion //!constructor/destructor
    
            #pragma region methods
   
            virtual void addToLightToUseBuffer(std::vector<Engine::Resources::light>& lb) noexcept
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

            virtual const Engine::Resources::AmbiantComponent&             getAmbient()               const noexcept { return ambientComp_; }
            virtual const Engine::Resources::DiffuseComponent&             getDiffuse()               const noexcept { return diffuseComp_; }
            virtual const Engine::Resources::SpecularComponent&            getSpecular()              const noexcept { return specularComp_; }
            virtual bool                                                    isEnable()                 const noexcept { return isEnable_; }

            /**
             * @brief Get the Lights To Use object converte in alignas struct to be load in block in shader
             * 
             * @return std::vector<Engine::LowRenderer::light> 
             */
            static
            std::vector<Engine::Resources::light> getLightsToUseInAlignasStruct ();

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

            Engine::Resources::AmbiantComponent        ambientComp_;
            Engine::Resources::DiffuseComponent        diffuseComp_;
            Engine::Resources::SpecularComponent       specularComp_;

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
