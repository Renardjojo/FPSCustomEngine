#include "GE/LowRenderer/Light/light.hpp"
#include "GE/LowRenderer/Light/pointLight.hpp"
#include "GE/LowRenderer/Light/spotLight.hpp"
#include <algorithm>

#ifndef DNEDITOR
#include "imgui/imgui.h"
#endif

using namespace Engine::Core::Maths;
using namespace Engine::LowRenderer;
using namespace Engine::Ressources;

Light::Light (Engine::Ressources::GameObject &      refGameObject, 
                const AmbiantComponent&             ambient, 
                const DiffuseComponent&             diffuse, 
                const SpecularComponent&            specular)
    :   Component          {refGameObject},
        ambientComp_       (ambient),
        diffuseComp_       (diffuse),
        specularComp_      (specular),
        isEnable_          (false)
{
    _name = __FUNCTION__;
}

Light::Light (Engine::Ressources::GameObject &refGameObject, const LightCreateArg&    arg)
    :   Component          {refGameObject},
        ambientComp_       (arg.ambient),
        diffuseComp_       (arg.diffuse),
        specularComp_      (arg.specular),
        isEnable_          (false)
{
    _name = __FUNCTION__;
}

void Light::enable (bool flag) throw() 
{
    if(isEnable_ == flag)
        return;
    else    
        isEnable_ = flag;

    for (std::vector<Engine::LowRenderer::Light*>::iterator it = lightsToUse.begin(); it != lightsToUse.end(); it++)
    {
        if (this == (*it))
        {
            if (!isEnable_)
                lightsToUse.erase(it);
            return;
        }
    }

    if (isEnable_)
        lightsToUse.push_back(this);
}

std::vector<Engine::Ressources::light> Light::getLightsToUseInAlignasStruct ()
{
    std::vector<Engine::Ressources::light> lightBufferRtn;

    for(Light* light : lightsToUse)
    {
        light->addToLightToUseBuffer(lightBufferRtn);
    }

    return lightBufferRtn;
}

#ifndef DNEDITOR
void Light::serializeOnEditor () noexcept
{
    ImGui::Text("Is activate :"); ImGui::SameLine();
    bool newEnable = isEnable_;
    if (ImGui::Checkbox("##isEnableLight", &newEnable))
    {
        enable (newEnable);
    }

    ImGui::ColorEdit4("Ambiant component", ambientComp_.e);
    ImGui::ColorEdit4("Diffuse component", diffuseComp_.e);
    ImGui::ColorEdit4("Specular component", specularComp_.e);
}
#endif

std::vector<Engine::LowRenderer::Light*> Light::lightsToUse(0);