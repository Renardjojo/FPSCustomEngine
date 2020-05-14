#ifndef __SCENE_SETUP_HPP__
#define __SCENE_SETUP_HPP__

#include "GE/Core/DataStructure/graph.hpp"
#include "GE/Ressources/scene.hpp"
#include "GE/LowRenderer/entity.hpp"
#include "GE/Ressources/ressourcesManager.hpp"
#include "GE/GE.hpp"

#include "save/rapidxml-1.13/rapidxml.hpp"
#include "save/rapidxml-1.13/rapidxml_print.hpp"
#include "save/rapidxml-1.13/rapidxml_utils.hpp"
#include <fstream>
#include <sstream>

using namespace rapidxml;


void setupScene(Engine::Ressources::Scene& scene, Engine::GE& gameEngine, const char* filePath);

void initEntity(Engine::Ressources::Scene& scene, Engine::Ressources::GameObject& parent, Engine::GE& gameEngine, xml_node<>* node);

void saveScene(Engine::Ressources::Scene& scene, Engine::GE& gameEngine, const char* filePath);

void saveEntity(Engine::Ressources::GameObject& gameObjectParent, Engine::GE& gameEngine, xml_document<>& doc, xml_node<>* nodeParent);


#endif