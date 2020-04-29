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

namespace Engine::LowRenderer
{
    typedef struct S_DirectionnalLightCreateArg
    {
        const Engine::Core::Maths::Vec3&                        direction;
        const Engine::Resources::AmbiantComponent&             ambient; 
        const Engine::Resources::DiffuseComponent&             diffuse;
        const Engine::Resources::SpecularComponent&            specular;

        const char*                         name;

    } DirectionnalLightCreateArg;

    class DirectionnalLight
        : public Light
    {
        public:
    
            #pragma region constructor/destructor
    
            DirectionnalLight ()						            = default;

            DirectionnalLight ( const DirectionnalLightCreateArg& arg)
                        :   Light               (arg.direction.getNormalize(), arg.ambient, arg.diffuse, arg.specular, arg.name)
            {}

                        
            DirectionnalLight ( const Engine::Core::Maths::Vec3&                direction,
                         const Engine::Resources::AmbiantComponent&            ambient, 
                         const Engine::Resources::DiffuseComponent&            diffuse, 
                         const Engine::Resources::SpecularComponent&           specular,
                         const char*                                            name)
            :   Light               (direction.getNormalize(), ambient, diffuse, specular, name)
            {}
            
            DirectionnalLight (const DirectionnalLight& other)		    = default;
            DirectionnalLight (DirectionnalLight&& other)			    = default;
            virtual ~DirectionnalLight ()				                = default;
    
            #pragma endregion //!constructor/destructor
    
            #pragma region methods


            virtual void addToLightToUseBuffer(std::vector<Engine::Resources::light>& lb) noexcept override
            {
                lb.push_back({  ambientComp_, 
                                diffuseComp_, 
                                specularComp_, 
                                position_, 3.f,
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

            #pragma endregion //!attribut

            #pragma region static attribut
            
            #pragma endregion //! static attribut
    
            #pragma region methods
            #pragma endregion //!methods
    
        private:
    
    };
}

#endif // _GE_DIRECTIONNAL_LIGHT_H