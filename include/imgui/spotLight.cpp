
#include "GE/LowRenderer/Light/spotLight.hpp"

#ifndef DNEDITOR
#include "imgui/imgui.h"
#endif

using namespace Engine::LowRenderer;
using namespace Engine::Ressources;

SpotLight::SpotLight ( GameObject &            refGameObject,
            const AmbiantComponent&            ambient, 
            const DiffuseComponent&            diffuse, 
            const SpecularComponent&           specular,
            float                              constant, 
            float                              linear, 
            float                              quadratic,
            const Engine::Core::Maths::Vec3&   direction,
            float                              cutOff,
            float                              cutOffExponent, 
            bool                                isEnable)

:   PointLight          (refGameObject, ambient, diffuse, specular, constant, linear, quadratic, isEnable),
    direction_          (direction),
    cutOff_             (cosf(cutOff * M_PI / 180.f)),
    cutOffExponent_     (cosf(cutOffExponent * M_PI / 180.f))
{
    _name = __FUNCTION__;
}

SpotLight::SpotLight (GameObject & refGameObject, SpotLightCreateArg arg)
:   PointLight          (refGameObject, arg.ambient, arg.diffuse, arg.specular, arg.constant, arg.linear, arg.quadratic, arg.isEnable),
    direction_          (arg.direction),
    cutOff_             (cosf(arg.cutOff * M_PI / 180.f)),
    cutOffExponent_     (cosf(arg.cutOffExponent * M_PI / 180.f))
{
    _name = __FUNCTION__;
}

SpotLight::SpotLight (GameObject &refGameObject, const std::vector<std::string>& params)
            :   PointLight          (refGameObject, params),
                direction_          {std::stof(params[16]), std::stof(params[17]), std::stof(params[18])},
                cutOff_             {std::stof(params[19])},
                cutOffExponent_     {std::stof(params[20])} 
{
    _name = __FUNCTION__;
}

void SpotLight::addToLightToUseBuffer(std::vector<light>& lb) noexcept
{
    lb.push_back({  ambientComp_, 
                    diffuseComp_, 
                    specularComp_,
                    getGameObject().getGlobalPosition(), 2.f,
                    constant_, linear_, quadratic_, cutOffExponent_,
                    direction_, cutOff_});
}

void SpotLight::save(xml_document<> &doc, xml_node<> *nodeParent)
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
    newNode->append_attribute(doc.allocate_attribute("isEnable", doc.allocate_string(std::to_string(isEnable_).c_str())));
    newNode->append_attribute(doc.allocate_attribute("direction0", doc.allocate_string(std::to_string(direction_.e[0]).c_str())));
    newNode->append_attribute(doc.allocate_attribute("direction1", doc.allocate_string(std::to_string(direction_.e[1]).c_str())));
    newNode->append_attribute(doc.allocate_attribute("direction2", doc.allocate_string(std::to_string(direction_.e[2]).c_str())));
    newNode->append_attribute(doc.allocate_attribute("cutOff", doc.allocate_string(std::to_string(cutOff_).c_str())));
    newNode->append_attribute(doc.allocate_attribute("cutOffExponent", doc.allocate_string(std::to_string(cutOffExponent_).c_str())));


    nodeParent->append_node(newNode);
}

#ifndef DNEDITOR
void SpotLight::serializeOnEditor () noexcept
{
    PointLight::serializeOnEditor();

    bool isTouch = false;

    ImGui::Text("Direction :");
    ImGui::Columns(3);
    ImGui::Text("X :"); ImGui::SameLine();
    isTouch |= ImGui::DragFloat("##dirSpotLightX", &direction_.x, 0.1f);
    ImGui::NextColumn();
    ImGui::Text("Y :"); ImGui::SameLine();
    isTouch |= ImGui::DragFloat("##dirSpotLightY", &direction_.y, 0.1f);
    ImGui::NextColumn();
    ImGui::Text("Z :"); ImGui::SameLine();
    isTouch |= ImGui::DragFloat("##dirSpotLightZ", &direction_.z, 0.1f);
    ImGui::Columns(1);

    if (isTouch)
        direction_.normalize();

    ImGui::Spacing();
    ImGui::Text("CutOff :"); ImGui::SameLine(); ImGui::DragFloat("##cutOffPoinLighttX", &cutOff_, 0.1f);
    ImGui::Text("CutOff exponent :"); ImGui::SameLine(); ImGui::DragFloat("##cutOffExponentPoinLightY", &cutOffExponent_, 0.1f);
}
#endif