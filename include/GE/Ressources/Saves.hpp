//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 21 h 11

#ifndef _SAVES_H
#define _SAVES_H

#include <vector>
#include <iostream>
#include <fstream>

namespace Engine::Ressources::Save
{

    void initSavePaths(std::vector<std::string>& savePaths, const char* path);

} /*namespace */

#endif //_SAVES_H