//Project : Engine
//Editing by Six Jonathan
//Date : 09/01/2020 - 14 h 42

#ifndef _GE_TYPE_H
#define _GE_TYPE_H

#include "GE/Core/Maths/vec.hpp"
#include <string>
#include <vector>

namespace Engine::Ressources
{
    typedef struct S_indice
    {
        unsigned int iv, ivt, ivn;

    } Indice;

    struct ColorRGBA
    {
        float r, g, b, a;
    };

    typedef union
    {    
        struct {float kr, kg, kb, ki;};
		float e[4];
        Engine::Core::Maths::Vec4 rgbi;
        ColorRGBA                 rgba;

    } AmbiantComponent;

    typedef AmbiantComponent DiffuseComponent;
    typedef AmbiantComponent SpecularComponent;
    typedef AmbiantComponent Emission;

    struct MaterialAttrib
    {
        AmbiantComponent    ambient;
        DiffuseComponent    diffuse;
        SpecularComponent   specular;
        Emission            emission;
        float               shininess;

        std::string         nameAmbiantTexture;
        std::string         nameDiffuseTexture;
        std::string         nameSpecularTexture;

        std::string         id;
    };

    struct Shape
    {
        std::string                                 name;   
        std::vector<unsigned int>                   iv;
        std::vector<unsigned int>                   ivn;
        std::vector<unsigned int>                   ivt;
        std::string                                 material_ids;
    };

    struct Attrib
    {
        std::string                                    objName;
        std::vector<::Engine::Core::Maths::Vec3>       vBuffer;
        std::vector<::Engine::Core::Maths::Vec2>       vtBuffer;
        std::vector<::Engine::Core::Maths::Vec3>       vnBuffer;

        void clear()
        {
            objName.clear();
            vBuffer.clear();
            vtBuffer.clear();
            vnBuffer.clear();
        }
    };

    struct Size
    {
        int width, heigth;
    };
}

#endif // _GE_TYPE_H
