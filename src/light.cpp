#include "GE/LowRenderer/Light/light.hpp"
#include "GE/LowRenderer/Light/pointLight.hpp"
#include "GE/LowRenderer/Light/spotLight.hpp"
#include <algorithm>

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

}

Light::Light (Engine::Ressources::GameObject &refGameObject, const LightCreateArg&    arg)
    :   Component          {refGameObject},
        ambientComp_       (arg.ambient),
        diffuseComp_       (arg.diffuse),
        specularComp_      (arg.specular),
        isEnable_          (false)
{}

void Light::enable (bool flag) throw() 
{
    if(isEnable_ == flag)
        return;
    else    
        isEnable_ = flag;

    //auto for iterator of Light* in vector
    auto it = std::find(lightsToUse.begin(), lightsToUse.end(), this);

    if(it == lightsToUse.end()) //if elem is store
    {
        if(isEnable_)
        {
            //load into static vector to use
            lightsToUse.push_back(this);
        }
        else
        {
            lightsToUse.erase(it);
        }
    }
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

std::vector<Engine::LowRenderer::Light*> Light::lightsToUse(0);