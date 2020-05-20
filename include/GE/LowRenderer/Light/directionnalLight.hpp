//Project : Engine
//Editing by Six Jonathan
//Date : 16/01/2020 - 10 h 02

#ifndef _GE_DIRECTIONNAL_LIGHT_H
#define _GE_DIRECTIONNAL_LIGHT_H

#include <cmath>
#include <vector>

#include "glad/glad.h"
#include "GE/Core/Maths/vec.hpp"
#include "GE/LowRenderer/entity.hpp"
#include "GE/LowRenderer/Light/light.hpp"
#include "GE/Ressources/GameObject.hpp"

namespace Engine::LowRenderer
{
    typedef struct S_DirectionnalLightCreateArg
    {
        const Engine::Core::Maths::Vec3&                        direction;
        const Engine::Ressources::AmbiantComponent&             ambient; 
        const Engine::Ressources::DiffuseComponent&             diffuse;
        const Engine::Ressources::SpecularComponent&            specular;

    } DirectionnalLightCreateArg;

    class DirectionnalLight
        : public Light
    {
        public:
    
            #pragma region constructor/destructor
    
            DirectionnalLight ()						            = default;

            DirectionnalLight (Engine::Ressources::GameObject & refGameObject, const DirectionnalLightCreateArg& arg)
                        :   Light               {refGameObject, arg.ambient, arg.diffuse, arg.specular},
                            _direction          {arg.direction.getNormalize()}
            {}

                        
            DirectionnalLight ( Engine::Ressources::GameObject &                       refGameObject,
                                const Engine::Core::Maths::Vec3&                       direction,
                                const Engine::Ressources::AmbiantComponent&            ambient, 
                                const Engine::Ressources::DiffuseComponent&            diffuse, 
                                const Engine::Ressources::SpecularComponent&           specular)
            :   Light               {refGameObject, ambient, diffuse, specular},
                _direction          {direction.getNormalize()}
            {}
            
            DirectionnalLight (const DirectionnalLight& other)		    = default;
            DirectionnalLight (DirectionnalLight&& other)			    = default;
            virtual ~DirectionnalLight ()				                = default;
    
            #pragma endregion //!constructor/destructor
    
            #pragma region methods


            virtual void addToLightToUseBuffer(std::vector<Engine::Ressources::light>& lb) noexcept override
            {
                lb.push_back({  ambientComp_, 
                                diffuseComp_, 
                                specularComp_, 
                                _direction, 3.f,
                                0.f, 0.f, 0.f, 0.f,
                                {0.f, 0.f, 0.f}, 0.f});
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
            Engine::Core::Maths::Vec3 _direction;
            #pragma endregion //!attribut

            #pragma region static attribut
            
            #pragma endregion //! static attribut
    
            #pragma region methods
            #pragma endregion //!methods
    
        private:
    
    };
}

#endif // _GE_DIRECTIONNAL_LIGHT_H