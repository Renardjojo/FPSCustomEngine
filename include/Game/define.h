//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-04 - 18 h 56

#ifndef _DEFINE_H
#define _DEFINE_H

#define WIDTH 1680
#define HEIGHT 945

namespace Engine 
{
    enum E_GAME_STATE
    {
        STARTING = 0,
        RUNNING,
        PAUSE,
        OPTION,
        STARTSAVE,
        LOADSAVE,
        DEAD,
        EXIT
    };

} /*namespace */
#endif //_DEFINE_H