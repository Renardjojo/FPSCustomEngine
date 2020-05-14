#include "GE/Ressources/Saves.hpp"

void Engine::Ressources::Save::initSavePaths(std::vector<std::string>& savePaths, const char* path)
{
    std::string head;

    std::ifstream file(path);

    if(!file.is_open())
    {
        std::cout << "FAIL TO READ SAVE FILE" << std::endl; // TODO: assert
        return;
    }

    while(file.good())
    {
        file >> head;

        savePaths.push_back(head);
    }

    file.close();
}
