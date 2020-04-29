//Project : Engine
//Editing by Six Jonathan
//Date : 08/01/2020 - 10 h 43

#ifndef _GE_OBJ_PARSER_H
#define _GE_OBJ_PARSER_H

#include <string>
#include <vector>

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Parsers/mtlParser.hpp"
#include "GE/Resources/type.hpp"

namespace Engine::Core::Parsers
{
    /**
     * @brief Parse Obj file and fill buffer if arg not Null
     * 
     * @param objPath 
     * @param attrib 
     * @param shape 
     * @param materials 
     */
    extern void loadObjWithMTL (const char*                                         objPath,
                                Engine::Resources::Attrib*                         attrib,
                                std::vector<Engine::Resources::Shape>*             shape,
                                std::vector<Engine::Resources::MaterialAttrib>*    materials) noexcept;

} // namespace Engine::Core::Parsers

#endif //_GE_OBJ_PARSER_H