//Project : Engine
//Editing by Six Jonathan
//Date : 16/01/2020 - 10 h 01

#ifndef _GE_SPOT_LIGHT_H
#define _GE_SPOT_LIGHT_H

#include <cmath>
#include <vector>

#include "glad/glad.h"
#include "GE/Core/Maths/vec.hpp"
#include "GE/LowRenderer/entity.hpp"
#include "GE/LowRenderer/Light/pointLight.hpp"
#include "GE/Ressources/GameObject.hpp"

namespace Engine::LowRenderer
{
    typedef struct S_SpotLightCreateArg
    {
        const Engine::Ressources::AmbiantComponent&             ambient; 
        const Engine::Ressources::DiffuseComponent&             diffuse;
        const Engine::Ressources::SpecularComponent&            specular;

        float                               constant; 
        float                               linear;
        float                               quadratic;

        const Engine::Core::Maths::Vec3&   direction;
        float                              cutOff;
        float                              cutOffExponent;

    } SpotLightCreateArg;

    class SpotLight
        : public PointLight
    {
        public:
    
            #pragma region constructor/destructor
    
            SpotLight ()						= default;

            /**
             * @brief Construct a new Spot Light object
             * 
             * @param pos 
             * @param ambient 
             * @param diffuse 
             * @param specular 
             * @param constant 
             * @param linear 
             * @param quadratic 
             * @param direction 
             * @param cutOff            : in degres : specifies the spotlight's radius
             * @param cutOffExponent    : in degres : specifies the spotlight's radius attenuation
             * @param name 
             */
            SpotLight ( Engine::Ressources::GameObject &                       refGameObject,
                        const Engine::Ressources::AmbiantComponent&            ambient, 
                        const Engine::Ressources::DiffuseComponent&            diffuse, 
                        const Engine::Ressources::SpecularComponent&           specular,
                        float                              constant, 
                        float                              linear, 
                        float                              quadratic,
                        const Engine::Core::Maths::Vec3&   direction,
                        float                              cutOff,
                        float                              cutOffExponent,
                        const char*                        name)
            :   PointLight          (refGameObject, ambient, diffuse, specular, constant, linear, quadratic),
                direction_          (direction),
                cutOff_             (cosf(cutOff * M_PI / 180.f)),
                cutOffExponent_     (cosf(cutOffExponent * M_PI / 180.f))
            {}

            SpotLight (Engine::Ressources::GameObject & refGameObject, SpotLightCreateArg arg)
            :   PointLight          (refGameObject, arg.ambient, arg.diffuse, arg.specular, arg.constant, arg.linear, arg.quadratic),
                direction_          (arg.direction),
                cutOff_             (cosf(arg.cutOff * M_PI / 180.f)),
                cutOffExponent_     (cosf(arg.cutOffExponent * M_PI / 180.f))
            {}
            
            SpotLight (const SpotLight& other)		= default;
            SpotLight (SpotLight&& other)			= default;
            virtual ~SpotLight ()				    = default;
    
            #pragma endregion //!constructor/destructor
    
            #pragma region methods

            virtual void addToLightToUseBuffer(std::vector<Engine::Ressources::light>& lb) noexcept override
            {
                lb.push_back({  ambientComp_, 
                                diffuseComp_, 
                                specularComp_,
                                getGameObject().getGlobalPosition(), 2.f,
                                constant_, linear_, quadratic_, cutOffExponent_,
                                direction_, cutOff_});
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

            Engine::Core::Maths::Vec3 direction_; 
            float cutOff_;              //specifies the spotlight's radius.
            float cutOffExponent_;

            #pragma endregion //!attribut

            #pragma region static attribut
            
            #pragma endregion //! static attribut
    
            #pragma region methods
            #pragma endregion //!methods
    
        private:
    
    };
}

#endif // _GE_SPOT_LIGHT_H