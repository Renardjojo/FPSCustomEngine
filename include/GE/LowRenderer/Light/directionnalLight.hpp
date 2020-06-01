//Project : Engine
//Editing by Six Jonathan
//Date : 16/01/2020 - 10 h 02

#ifndef _GE_DIRECTIONNAL_LIGHT_H
#define _GE_DIRECTIONNAL_LIGHT_H

#include <vector>

#ifndef DNEDITOR
#include "imgui/imgui.h"
#endif

#include "GE/Core/Maths/vec.hpp"
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
        bool isEnable                                           {true};

    } DirectionnalLightCreateArg;

    class DirectionnalLight
        : public Light
    {
        public:
    
            #pragma region constructor/destructor
    
            DirectionnalLight ()						            = default;

            DirectionnalLight (Engine::Ressources::GameObject & refGameObject, const DirectionnalLightCreateArg& arg);

                        
            DirectionnalLight ( Engine::Ressources::GameObject &                       refGameObject,
                                const Engine::Core::Maths::Vec3&                       direction,
                                const Engine::Ressources::AmbiantComponent&            ambient, 
                                const Engine::Ressources::DiffuseComponent&            diffuse, 
                                const Engine::Ressources::SpecularComponent&           specular,
                                bool isEnable = true);
            
            DirectionnalLight (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params);

            DirectionnalLight (const DirectionnalLight& other)		    = default;
            DirectionnalLight (DirectionnalLight&& other)			    = default;
            virtual ~DirectionnalLight ()				                = default;
    
            #pragma endregion //!constructor/destructor
    
            #pragma region methods


            virtual void addToLightToUseBuffer(std::vector<Engine::Ressources::light>& lb) noexcept override;
            
            void save(xml_document<>& doc, xml_node<>* nodeParent);

            #ifndef DNEDITOR
            virtual void serializeOnEditor () noexcept override;
            #endif

            void setDirection( const  Engine::Core::Maths::Vec3& newDirection) noexcept { _direction = newDirection.getNormalize();}

            #pragma endregion //!methods

    
        protected:
    
            #pragma region attribut
            Engine::Core::Maths::Vec3 _direction;
            #pragma endregion //!attribut
    
        private:
    
    };
}

#endif // _GE_DIRECTIONNAL_LIGHT_H