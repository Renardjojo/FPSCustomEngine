
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
            float                              cutOffExponent)

:   PointLight          (refGameObject, ambient, diffuse, specular, constant, linear, quadratic),
    direction_          (direction),
    cutOff_             (cosf(cutOff * M_PI / 180.f)),
    cutOffExponent_     (cosf(cutOffExponent * M_PI / 180.f))
{}

SpotLight::SpotLight (GameObject & refGameObject, SpotLightCreateArg arg)
:   PointLight          (refGameObject, arg.ambient, arg.diffuse, arg.specular, arg.constant, arg.linear, arg.quadratic),
    direction_          (arg.direction),
    cutOff_             (cosf(arg.cutOff * M_PI / 180.f)),
    cutOffExponent_     (cosf(arg.cutOffExponent * M_PI / 180.f))
{}


void SpotLight::addToLightToUseBuffer(std::vector<light>& lb) noexcept
{
    lb.push_back({  ambientComp_, 
                    diffuseComp_, 
                    specularComp_,
                    getGameObject().getGlobalPosition(), 2.f,
                    constant_, linear_, quadratic_, cutOffExponent_,
                    direction_, cutOff_});
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