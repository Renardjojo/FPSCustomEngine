#include "GE/Resources/material.hpp"
#include "GE/Core/Debug/log.hpp"

using namespace Engine::Core::Debug;
using namespace Engine::Resources;

Material::Material (const MaterialAndTextureCreateArg& arg)
    :   comp_                           (arg.comp_),
        pDiffuseTexturetexture_         (),
        name_                           (arg.name_)
{
    if (arg.pathDiffuseTexture == nullptr)
    {
        pDiffuseTexturetexture_ = nullptr;
    }
    else
    {
        TextureCreateArg textArg;
        textArg.path            = arg.pathDiffuseTexture;
        textArg.filterType      = arg.filterType;
        textArg.loadInGPU       = arg.loadInGPU;
        textArg.wrapType        = arg.wrapType;
        textArg.flipTexture     = arg.flipTexture;
        
        pDiffuseTexturetexture_ = std::make_unique<Texture>(textArg);
    }
}

Material::Material (const MaterialAttrib& arg)
    :   comp_                           {   arg.ambient, 
                                            arg.diffuse, 
                                            arg.specular,
                                            arg.shininess},
        pDiffuseTexturetexture_         (std::make_unique<Texture>((std::string(PATH_TEXTURE_RESSORUCE) + arg.nameDiffuseTexture).c_str())),
        name_                           (arg.id)
{}

Material::Material (MaterialCreateArg& arg)
    :   comp_                           (arg.comp),
        pDiffuseTexturetexture_         (std::move(arg.pTexture)),
        name_                           (arg.name)
{}