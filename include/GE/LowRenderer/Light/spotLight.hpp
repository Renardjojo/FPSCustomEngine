//Project : Engine
//Editing by Six Jonathan
//Date : 16/01/2020 - 10 h 01

#ifndef _GE_SPOT_LIGHT_H
#define _GE_SPOT_LIGHT_H

#include <vector>

#ifndef DNEDITOR
#include "imgui/imgui.h"
#endif

#include "GE/Core/Maths/vec.hpp"
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
                        float                              cutOffExponent);

            SpotLight (Engine::Ressources::GameObject & refGameObject, SpotLightCreateArg arg);
            
            SpotLight (const SpotLight& other)		= default;
            SpotLight (SpotLight&& other)			= default;
            virtual ~SpotLight ()				    = default;
    
            #pragma endregion //!constructor/destructor
    
            #pragma region methods

            virtual void addToLightToUseBuffer(std::vector<Engine::Ressources::light>& lb) noexcept override;

            #pragma endregion //!methods
    
            #ifndef DNEDITOR
            virtual void serializeOnEditor () noexcept override;
            #endif
    
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