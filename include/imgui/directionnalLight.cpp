#include "GE/LowRenderer/Light/directionnalLight.hpp"

#include <string>

#ifndef DNEDITOR
#include "imgui/imgui.h"
#endif

using namespace Engine::LowRenderer;
using namespace Engine::Core::Maths;
using namespace Engine::Ressources;


DirectionnalLight::DirectionnalLight (GameObject & refGameObject, const DirectionnalLightCreateArg& arg)
            :   Light               {refGameObject, arg.ambient, arg.diffuse, arg.specular, arg.isEnable},
                _direction          {arg.direction.getNormalize()}
{
    _name = __FUNCTION__;
}

            
DirectionnalLight::DirectionnalLight ( GameObject &                       refGameObject,
                                        const Engine::Core::Maths::Vec3&   direction,
                                        const AmbiantComponent&            ambient, 
                                        const DiffuseComponent&            diffuse, 
                                        const SpecularComponent&           specular,
                                        bool isEnable)
:   Light               {refGameObject, ambient, diffuse, specular, isEnable},
    _direction          {direction.getNormalize()}
{
    _name = __FUNCTION__;
}

DirectionnalLight::DirectionnalLight (GameObject &refGameObject, const std::vector<std::string>& params)
:   Light       (refGameObject,
                AmbiantComponent{std::stof(params[0]), std::stof(params[1]), std::stof(params[2]), std::stof(params[3])}, 
                AmbiantComponent{std::stof(params[4]), std::stof(params[5]), std::stof(params[6]), std::stof(params[7])}, 
                AmbiantComponent{std::stof(params[8]), std::stof(params[9]), std::stof(params[10]), std::stof(params[11])},
                static_cast<bool>(std::stof(params[15]))),
    _direction  {Engine::Core::Maths::Vec3{std::stof(params[12]), std::stof(params[13]), std::stof(params[14])}} 
{
    _name = __FUNCTION__;
}

void DirectionnalLight::addToLightToUseBuffer(std::vector<light>& lb) noexcept
{
    lb.push_back({  ambientComp_, 
                    diffuseComp_, 
                    specularComp_, 
                    _direction, 3.f,
                    0.f, 0.f, 0.f, 0.f,
                    {0.f, 0.f, 0.f}, 0.f});
}

void DirectionnalLight::save(xml_document<>& doc, xml_node<>* nodeParent)
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
    newNode->append_attribute(doc.allocate_attribute("isEnable", doc.allocate_string(std::to_string(isEnable_).c_str())));
    
    nodeParent->append_node(newNode);
}

#ifndef DNEDITOR
void DirectionnalLight::serializeOnEditor () noexcept
{
    Light::serializeOnEditor();

    bool isTouch = false;

    ImGui::Text("Direction :");
    ImGui::Columns(3);
    ImGui::Text("X :"); ImGui::SameLine();
    isTouch |= ImGui::DragFloat("##dirLightX", &_direction.x, 0.1f);
    ImGui::NextColumn();
    ImGui::Text("Y :"); ImGui::SameLine();
    isTouch |= ImGui::DragFloat("##dirLightY", &_direction.y, 0.1f);
    ImGui::NextColumn();
    ImGui::Text("Z :"); ImGui::SameLine();
    isTouch |= ImGui::DragFloat("##dirLightZ", &_direction.z, 0.1f);
    ImGui::Columns(1);

    if (isTouch)
        _direction.normalize();
}
#endif
