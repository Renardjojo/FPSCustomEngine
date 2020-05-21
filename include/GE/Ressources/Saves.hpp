#ifndef __SAVES_HPP__
#define __SAVES_HPP__

#include "GE/Core/DataStructure/graph.hpp"
#include "GE/Ressources/scene.hpp"
#include "GE/LowRenderer/entity.hpp"
#include "GE/Ressources/ressourcesManager.hpp"
#include "GE/GE.hpp"

#include "save/rapidxml-1.13/rapidxml.hpp"
#include "save/rapidxml-1.13/rapidxml_print.hpp"
#include "save/rapidxml-1.13/rapidxml_utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace rapidxml;


namespace Engine::Ressources::Save
{
    void initSavePaths(std::vector<std::string>& savePaths, const char* path);

    void addSavePath(std::vector<std::string>& savePaths, const char* path);

    void setupScene(Engine::Ressources::Scene& scene, Engine::GE& gameEngine, const char* filePath);

    void initEntity(Engine::Ressources::Scene& scene, Engine::Ressources::GameObject& parent, Engine::GE& gameEngine, xml_node<>* node);

    void saveScene(Engine::Ressources::Scene& scene, Engine::GE& gameEngine, const char* filePath);

    void saveEntity(Engine::Ressources::GameObject& gameObjectParent, xml_document<>& doc, xml_node<>* nodeParent);

    void loadPrefab(Scene& scene, GameObject& parent, Engine::GE& gameEngine, const char* filePath);

    void createPrefab(GameObject& gameObject, std::string prefabName);
} 



#endif