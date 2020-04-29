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
        const Engine::Resources::AmbiantComponent&             ambient; 
        const Engine::Resources::DiffuseComponent&             diffuse;
        const Engine::Resources::SpecularComponent&            specular;

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
                         const Engine::Resources::AmbiantComponent&            ambient, 
                         const Engine::Resources::DiffuseComponent&            diffuse, 
                         const Engine::Resources::SpecularComponent&           specular,
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


            virtual void addToLightToUseBuffer(std::vector<Engine::Resources::light>& lb) noexcept override
            {
                lb.push_back({  ambientComp_, 
                                diffuseComp_, 
                                specularComp_, 
                                position_, 1.f,
                                constant_, linear_, quadratic_, 0.f,
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

            float constant_, linear_, quadratic_;

            #pragma endregion //!attribut

            #pragma region static attribut
            
            #pragma endregion //! static attribut
    
            #pragma region methods
            #pragma endregion //!methods
    
        private:
    
    };
}

#endif // _GE_POINT_LIGHT_H