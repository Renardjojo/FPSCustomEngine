//Project : Engine
//Editing by Six Jonathan
//Date : 07/01/2020 - 16 h 39

#ifndef _GE_ASSERT_H
#define _GE_ASSERT_H

#ifndef NDEBUG
#include "GE/Core/Debug/log.hpp"
#include <string>
#endif

//Engine::Core::Debug::[..]
namespace Engine::Core::Debug 
    {
    #ifdef NDEBUG

    #define GE_assert(expr)
    #define GE_assertInfo(expr, msg)

    #else /* Not NDEBUG.  */

    #define GE_assert(expr) \
        if (expr) {}	    \
        else                \
        {                   \
            Engine::Core::Debug::SLog::logError((::std::string(FRED("Assertion")) + " in function " + __FUNCTION__ + " " +  __FILE__ + ":" + ::std::to_string(__LINE__) + "\nExpression \"" + BOLD(#expr) + "\" == false.\n").c_str()); \
            exit (3);           \
        }

    #define GE_assertInfo(expr, msg) \
        if (expr) {}	    \
        else                \
        {                   \
            Engine::Core::Debug::SLog::logError((::std::string(FRED("Assertion")) + " in function " + __FUNCTION__ + " " +  __FILE__ + ":" + ::std::to_string(__LINE__) + "\nExpression \"" + BOLD(#expr) + "\" == false.\n" + msg).c_str()); \
            exit (3);           \
        }

    #endif //NDEBUG
} // namespace Engine::Core::Debug

#endif // _GE_ASSERT_H