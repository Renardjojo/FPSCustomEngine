//Project : Engine
//Editing by Six Jonathan
//Date : 17/01/2020 - 18 h 12

#ifndef _GE_SHADER_TYPE_H
#define _GE_SHADER_TYPE_H

#include "GE/Core/Maths/vec.hpp"
#include "GE/Resources/type.hpp"

namespace Engine::Resources
{
    struct alignas(16) MaterialComponent
    {
        AmbiantComponent    ambient;
        DiffuseComponent    diffuse;
        SpecularComponent   specular;
        float               shininess;
    };

    // Same memory layout than 'struct light' in glsl shader
    // alignas to Vec4 at 16o. Pading must be add if alignas is not respected (i.e : Vec3 + pad)
    struct alignas(16) light
    {
        AmbiantComponent Ambient;
        DiffuseComponent Diffuse;
        SpecularComponent Specular;

        Engine::Core::Maths::Vec3 lightPosition; float lightype;

        //Point arg
        float constant, linear, quadratic, cutOffExponent; //Pad1 for alignas at 16o

        //Spot arg
        Engine::Core::Maths::Vec3 direction;
        float cutOff; // specifies the spotlight's radius.
    };
}

#endif // _GE_SHADER_TYPE_H