
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
            float                              cutOff,
            float                              cutOffExponent, 
            bool                               isEnable)

:   PointLight          (refGameObject, ambient, diffuse, specular, constant, linear, quadratic, isEnable),
    cutOff_             (cosf(cutOff * M_PI / 180.f)),
    cutOffExponent_     (cosf(cutOffExponent * M_PI / 180.f))
{
    _name = __FUNCTION__;
}

SpotLight::SpotLight (GameObject & refGameObject, SpotLightCreateArg arg)
:   PointLight          (refGameObject, arg.ambient, arg.diffuse, arg.specular, arg.constant, arg.linear, arg.quadratic, arg.isEnable),
    cutOff_             (cosf(arg.cutOff * M_PI / 180.f)),
    cutOffExponent_     (cosf(arg.cutOffExponent * M_PI / 180.f))
{
    _name = __FUNCTION__;
}

SpotLight::SpotLight (GameObject &refGameObject, const std::vector<std::string>& params)
            :   PointLight          (refGameObject, params),
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
                    _gameObject.getVecForward(), cutOff_});
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
    newNode->append_attribute(doc.allocate_attribute("cutOff", doc.allocate_string(std::to_string(cutOff_).c_str())));
    newNode->append_attribute(doc.allocate_attribute("cutOffExponent", doc.allocate_string(std::to_string(cutOffExponent_).c_str())));


    nodeParent->append_node(newNode);
}

#ifndef DNEDITOR
void SpotLight::serializeOnEditor () noexcept
{
    PointLight::serializeOnEditor();

    ImGui::Spacing();

    float acosCutOffDeg = acos(cutOff_) * 180.f / M_PI;
    float acosCutOffExpDeg = acos(cutOffExponent_) * 180.f / M_PI;

    ImGui::Text("CutOff :"); ImGui::SameLine();
    if (ImGui::DragFloat("##cutOffPoinLighttX", &acosCutOffDeg, 0.1f))
    {
        cutOff_ = cosf(acosCutOffDeg * M_PI / 180.f);
    }

    ImGui::Text("CutOff exponent :"); ImGui::SameLine(); 
    if (ImGui::DragFloat("##cutOffExponentPoinLightY", &acosCutOffExpDeg, 0.1f))
    {
        cutOffExponent_ = cosf(acosCutOffExpDeg * M_PI / 180.f);
    }
}
#endif