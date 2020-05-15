//Project : Engine
//Editing by Six Jonathan
//Date : 16/01/2020 - 10 h 02

#ifndef _GE_POINT_LIGHT_H
#define _GE_POINT_LIGHT_H

#include <cmath>
#include <vector>

#include "glad/glad.h"
#include "GE/Core/Maths/vec.hpp"
#include "GE/LowRenderer/entity.hpp"
#include "GE/LowRenderer/Light/light.hpp"

namespace Engine::LowRenderer
{
    typedef struct S_PointLightCreateArg
    {
        const Engine::Core::Maths::Vec3&    pos;
        const Engine::Ressources::AmbiantComponent&             ambient; 
        const Engine::Ressources::DiffuseComponent&             diffuse;
        const Engine::Ressources::SpecularComponent&            specular;

        float                               constant; 
        float                               linear;
        float                               quadratic;

        const char*                         name;

    } PointLightCreateArg;

    class PointLight
        : public Light
    {
        public:
    
            #pragma region constructor/destructor
    
            PointLight ()						            = default;

            PointLight ( const PointLightCreateArg& arg)
                        :   Light               (arg.pos, arg.ambient, arg.diffuse, arg.specular, arg.name),
                            constant_           (arg.constant),
                            linear_             (arg.linear),
                            quadratic_          (arg.quadratic)
            {}

                        
            PointLight ( const Engine::Core::Maths::Vec3&                       pos,
                         const Engine::Ressources::AmbiantComponent&            ambient, 
                         const Engine::Ressources::DiffuseComponent&            diffuse, 
                         const Engine::Ressources::SpecularComponent&           specular,
                         float                              constant, 
                         float                              linear, 
                         float                              quadratic, 
                         const char*                        name)
            :   Light               (pos, ambient, diffuse, specular, name),
                constant_           (constant),
                linear_             (linear),
                quadratic_          (quadratic)
            {}
            
            PointLight (const PointLight& other)		    = default;
            PointLight (PointLight&& other)			        = default;
            virtual ~PointLight ()				            = default;
    
            #pragma endregion //!constructor/destructor
    
            #pragma region methods


            virtual void addToLightToUseBuffer(std::vector<Engine::Ressources::light>& lb) noexcept override
            {
                lb.push_back({  ambientComp_, 
                                diffuseComp_, 
                                specularComp_, 
                                position_, 1.f,
                                constant_, linear_, quadratic_, 0.f,
                                {0.f, 0.f, 0.f}, 0.f});
            }
            

            #pragma endregion //!methods
    
            float getConstant() { return constant_; }
            float getLinear() { return linear_; }
            float getQuadratic() { return quadratic_; }

    
        protected:

            float constant_, linear_, quadratic_;
    
        private:
    
    };
}

#endif // _GE_POINT_LIGHT_H