//Project : Engine
//Editing by Six Jonathan
//Date : 16/01/2020 - 10 h 02

#ifndef _GE_POINT_LIGHT_H
#define _GE_POINT_LIGHT_H

#include <vector>

#ifndef DNEDITOR
#include "imgui/imgui.h"
#endif

#include "GE/Core/Maths/vec.hpp"
#include "GE/LowRenderer/Light/light.hpp"
#include "GE/Ressources/GameObject.hpp"

namespace Engine::LowRenderer
{
    typedef struct S_PointLightCreateArg
    {
        const Engine::Ressources::AmbiantComponent&             ambient; 
        const Engine::Ressources::DiffuseComponent&             diffuse;
        const Engine::Ressources::SpecularComponent&            specular;

        float                               constant; 
        float                               linear;
        float                               quadratic;

    } PointLightCreateArg;

    class PointLight
        : public Light
    {
        public:
    
            #pragma region constructor/destructor
    
            PointLight ()						            = default;

            PointLight (Engine::Ressources::GameObject &refGameObject, const PointLightCreateArg& arg);
                        
            PointLight ( Engine::Ressources::GameObject &                       refGameObject,
                         const Engine::Ressources::AmbiantComponent&            ambient, 
                         const Engine::Ressources::DiffuseComponent&            diffuse, 
                         const Engine::Ressources::SpecularComponent&           specular,
                         float                              constant, 
                         float                              linear, 
                         float                              quadratic);
            
            PointLight (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params);
            
            PointLight (const PointLight& other)		    = default;
            PointLight (PointLight&& other)			        = default;
            virtual ~PointLight ()				            = default;
    
            #pragma endregion //!constructor/destructor
    
            #pragma region methods


            virtual void addToLightToUseBuffer(std::vector<Engine::Ressources::light>& lb) noexcept override;
    
            void save(xml_document<> &doc, xml_node<> *nodeParent);

            #ifndef DNEDITOR
            virtual void serializeOnEditor () noexcept override;
            #endif

            #pragma endregion //!methods

        protected:

            float constant_, linear_, quadratic_;
    
        private:
    
    };
}

#endif // _GE_POINT_LIGHT_H