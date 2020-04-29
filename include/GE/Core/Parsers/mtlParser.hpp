//Project : Engine
//Editing by Six Jonathan
//Date : 20/01/2020 - 14 h 53

#ifndef _GE_MTL_PARSER_H
#define _GE_MTL_PARSER_H

#include <string>
#include <vector>

#include "GE/Resources/type.hpp"

#define PATH_MTL "./resources/material/"
#define EXTENSION_MTL ".mtl"

namespace Engine::Core::Parsers
{
    /**
     * @brief Parse file .mtl and return buffer of data parsing
     * 
     * @param materialPath 
     * @param materials
     */
    extern void loadMTL (std::string& materialPath, std::vector<Engine::Resources::MaterialAttrib>&  materials) noexcept;

} // namespace Engine::Core::Parsers

#endif // _GE_MTL_PARSER_H