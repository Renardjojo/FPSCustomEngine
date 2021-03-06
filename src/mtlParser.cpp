#include "GE/Core/Parsers/mtlParser.hpp"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include "GE/Core/Debug/assert.hpp"
#include "GE/Core/Debug/log.hpp"
#include "GE/Core/Parsers/parserTool.hpp"

using namespace Engine::Ressources;
using namespace Engine::Core::Maths;
using namespace Engine::Core::Debug;
using namespace Engine::Core::Parsers;

enum class E_ComponentType
{
    AMBIENT,
    DIFFUSE,
    SPECULAR,
    EMISSION
};

void parseComponent (std::string& line, std::vector<MaterialAttrib>& materials, E_ComponentType type)
{
    std::size_t cursorStart = line.find_first_not_of(" ", 2);
    std::size_t cursorEnd = line.find(" ", cursorStart);
    float x = parse<float>(line, cursorStart, cursorEnd - cursorStart);

    cursorStart = line.find_first_not_of(" ", cursorEnd);
    cursorEnd = line.find(" ", cursorStart);
    float y = parse<float>(line, cursorStart, cursorEnd - cursorStart);

    cursorStart = line.find_first_not_of(" ", cursorEnd);
    cursorEnd = line.find(" ", cursorStart);
    float z = parse<float>(line, cursorStart, cursorEnd - cursorStart);

    parse<float>(line, 2);

    switch (type)
    {
    case E_ComponentType::AMBIENT :
        materials.back().ambient.rgbi = {x, y, z, 1.f};
        break;

    case E_ComponentType::DIFFUSE :
        materials.back().diffuse.rgbi = {x, y, z, 1.f};
        break;

    case E_ComponentType::SPECULAR :
        materials.back().specular.rgbi = {x, y, z, 1.f};
        break;
    default:
        break;
    }
}

inline 
void parseTextureName (std::string& line, std::vector<MaterialAttrib>&  materials, E_ComponentType type)
{
    switch (type)
    {
    case E_ComponentType::AMBIENT :
        parseName(line, materials.back().nameAmbiantTexture);
        break;

    case E_ComponentType::DIFFUSE :
        parseName(line, materials.back().nameDiffuseTexture);
        break;

    case E_ComponentType::SPECULAR :
        parseName(line, materials.back().nameSpecularTexture);
        break;
    default:
        break;
    }
}

inline
void createNewMaterialAndParseName(std::string line, std::vector<MaterialAttrib>&  materials)
{
    materials.push_back({});
    parseName(line, materials.back().id);
}

inline
void parseLine(std::string line, std::vector<MaterialAttrib>&  materials)
{
    //Detect line type and parse this line
    if      (isTitle(line, "#"))            return;
    else if (isTitle(line, "newmtl"))       createNewMaterialAndParseName(line, materials);
    else if (isTitle(line, "Ka"))           parseComponent(line, materials, E_ComponentType::AMBIENT);
    else if (isTitle(line, "Kd"))           parseComponent(line, materials, E_ComponentType::DIFFUSE);
    else if (isTitle(line, "Ks"))           parseComponent(line, materials, E_ComponentType::SPECULAR);
    else if (isTitle(line, "Ns"))           materials.back().shininess = parse<float>(line, 2);
    else if (isTitle(line, "map_Ka"))       parseTextureName(line, materials, E_ComponentType::AMBIENT);
    else if (isTitle(line, "map_Kd"))       parseTextureName(line, materials, E_ComponentType::DIFFUSE);
    else if (isTitle(line, "map_Ks"))       parseTextureName(line, materials, E_ComponentType::SPECULAR);
    else                                    SLog::logWarning(std::string("MTL Parser cannot read this line : " + line).c_str());
}

void Engine::Core::Parsers::loadMTL (std::string& materialPath, std::vector<MaterialAttrib>&  materials) noexcept
{
    std::ifstream file(materialPath);

	if(!file)
	{
        SLog::logError((std::string("Error to found or read file : ") + materialPath).c_str());
        exit(1);
	}

    std::string line;
	while(getline(file >> std::ws, line))
	{
        if (line.size() < 2)
            continue;

        parseLine (line, materials);
    }
    
    SLog::log("Material parsing done");    
}