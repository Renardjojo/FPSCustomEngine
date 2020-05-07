#ifndef __XMLPARSE_HPP__
#define __XMLPARSE_HPP__

#include "GE/Core/DataStructure/graph.hpp"
#include "GE/Ressources/scene.hpp"
#include "GE/LowRenderer/entity.hpp"
#include "GE/Ressources/ressourcesManager.hpp"
#include "GE/GE.hpp"

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

using namespace rapidxml;


void setupScene(Engine::Ressources::Scene& scene, Engine::GE& gameEngine, const char* filePath);

void initEntity(Engine::Ressources::Scene& scene, Engine::Ressources::GameObject& parent, Engine::GE& gameEngine, xml_node<>* node);


#endif