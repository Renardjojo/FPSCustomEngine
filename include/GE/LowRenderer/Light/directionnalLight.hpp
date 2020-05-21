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
            
            DirectionnalLight (Engine::Ressources::GameObject &refGameObject, const std::vector<std::unique_ptr<std::string>>& params)
            :   Light       (refGameObject,
                            Engine::Ressources::AmbiantComponent{std::stof(*params[0]), std::stof(*params[1]), std::stof(*params[2]), std::stof(*params[3])}, 
                            Engine::Ressources::AmbiantComponent{std::stof(*params[4]), std::stof(*params[5]), std::stof(*params[6]), std::stof(*params[7])}, 
                            Engine::Ressources::AmbiantComponent{std::stof(*params[8]), std::stof(*params[9]), std::stof(*params[10]), std::stof(*params[11])}),
                _direction  {Engine::Core::Maths::Vec3{std::stof(*params[12]), std::stof(*params[13]), std::stof(*params[14])}} 
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
            
            void save(xml_document<>& doc, xml_node<>* nodeParent)
            {
                xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

                newNode->append_attribute(doc.allocate_attribute("type", "DirectionnalLight"));
                
                newNode->append_attribute(doc.allocate_attribute("ambient0", doc.allocate_string(std::to_string(ambientComp_.e[0]).c_str())));
                newNode->append_attribute(doc.allocate_attribute("ambient1", doc.allocate_string(std::to_string(ambientComp_.e[1]).c_str())));
                newNode->append_attribute(doc.allocate_attribute("ambient2", doc.allocate_string(std::to_string(ambientComp_.e[2]).c_str())));
                newNode->append_attribute(doc.allocate_attribute("ambient3", doc.allocate_string(std::to_string(ambientComp_.e[3]).c_str())));
                newNode->append_attribute(doc.allocate_attribute("diffuse0", doc.allocate_string(std::to_string(diffuseComp_.e[0]).c_str())));
                newNode->append_attribute(doc.allocate_attribute("diffuse1", doc.allocate_string(std::to_string(diffuseComp_.e[1]).c_str())));
                newNode->append_attribute(doc.allocate_attribute("diffuse2", doc.allocate_string(std::to_string(diffuseComp_.e[2]).c_str())));
                newNode->append_attribute(doc.allocate_attribute("diffuse3", doc.allocate_string(std::to_string(diffuseComp_.e[3]).c_str())));
                newNode->append_attribute(doc.allocate_attribute("specular0", doc.allocate_string(std::to_string(specularComp_.e[0]).c_str())));
                newNode->append_attribute(doc.allocate_attribute("specular1", doc.allocate_string(std::to_string(specularComp_.e[1]).c_str())));
                newNode->append_attribute(doc.allocate_attribute("specular2", doc.allocate_string(std::to_string(specularComp_.e[2]).c_str())));
                newNode->append_attribute(doc.allocate_attribute("specular3", doc.allocate_string(std::to_string(specularComp_.e[3]).c_str())));
                newNode->append_attribute(doc.allocate_attribute("dirX", doc.allocate_string(std::to_string(_direction.x).c_str())));
                newNode->append_attribute(doc.allocate_attribute("dirY", doc.allocate_string(std::to_string(_direction.y).c_str())));
                newNode->append_attribute(doc.allocate_attribute("dirZ", doc.allocate_string(std::to_string(_direction.z).c_str())));
                
                nodeParent->append_node(newNode);
            }

            #pragma endregion //!methods

    
        protected:
    
            #pragma region attribut
            Engine::Core::Maths::Vec3 _direction;
            #pragma endregion //!attribut
    
        private:
    
    };
}

#endif // _GE_DIRECTIONNAL_LIGHT_H