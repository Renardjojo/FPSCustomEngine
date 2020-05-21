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

            PointLight (Engine::Ressources::GameObject &refGameObject, const PointLightCreateArg& arg)
                        :   Light               (refGameObject, arg.ambient, arg.diffuse, arg.specular),
                            constant_           (arg.constant),
                            linear_             (arg.linear),
                            quadratic_          (arg.quadratic)
            {}
                        
            PointLight ( Engine::Ressources::GameObject &                       refGameObject,
                         const Engine::Ressources::AmbiantComponent&            ambient, 
                         const Engine::Ressources::DiffuseComponent&            diffuse, 
                         const Engine::Ressources::SpecularComponent&           specular,
                         float                              constant, 
                         float                              linear, 
                         float                              quadratic)
            :   Light               (refGameObject, ambient, diffuse, specular),
                constant_           (constant),
                linear_             (linear),
                quadratic_          (quadratic)
            {}
            
            PointLight (Engine::Ressources::GameObject &refGameObject, const std::vector<std::unique_ptr<std::string>>& params)
                        :   Light               (refGameObject, 
                                                Engine::Ressources::AmbiantComponent{std::stof(*params[0]), std::stof(*params[1]), std::stof(*params[2]), std::stof(*params[3])}, 
                                                Engine::Ressources::AmbiantComponent{std::stof(*params[4]), std::stof(*params[5]), std::stof(*params[6]), std::stof(*params[7])}, 
                                                Engine::Ressources::AmbiantComponent{std::stof(*params[8]), std::stof(*params[9]), std::stof(*params[10]), std::stof(*params[11])}), 
                            constant_           (std::stof(*params[12])),
                            linear_             (std::stof(*params[13])),
                            quadratic_          (std::stof(*params[14]))
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
                                _gameObject.getGlobalPosition(), 1.f,
                                constant_, linear_, quadratic_, 0.f,
                                {0.f, 0.f, 0.f}, 0.f});
            }
    
            void save(xml_document<> &doc, xml_node<> *nodeParent)
            {
                xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

                newNode->append_attribute(doc.allocate_attribute("type", "PointLight"));

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
                newNode->append_attribute(doc.allocate_attribute("constant", doc.allocate_string(std::to_string(constant_).c_str())));
                newNode->append_attribute(doc.allocate_attribute("linear", doc.allocate_string(std::to_string(linear_).c_str())));
                newNode->append_attribute(doc.allocate_attribute("quadriatic", doc.allocate_string(std::to_string(quadratic_).c_str())));

                nodeParent->append_node(newNode);
            }

            #pragma endregion //!methods

        protected:

            float constant_, linear_, quadratic_;
    
        private:
    
    };
}

#endif // _GE_POINT_LIGHT_H