#include "Game/Loot.hpp"

using namespace Game;
using namespace Engine::Core::Component;
using namespace Engine::Ressources;

Loot::Loot(Engine::Ressources::GameObject &gameObject)
    :   ScriptComponent    {gameObject}
{
    _name = __FUNCTION__;
}

Loot::Loot (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params)
    :   ScriptComponent    {refGameObject},
        _activationRadius   {std::stof(params[0])}
{
    _name = __FUNCTION__;
}

void Loot::start () noexcept
{
    _playerGO = &Engine::Ressources::Scene::getCurrentScene()->getGameObject("Players/Player1");
}

void Loot::update () noexcept
{
    //std::cout << (_playerGO->getGlobalPosition() - _gameObject.getGlobalPosition()).length() << "   " <<  _activationRadius << std::endl;
    if ((_playerGO->getGlobalPosition() - _gameObject.getGlobalPosition()).length() <= _activationRadius)
    {
        effect();
        _gameObject.destroy();
    }
}


void Loot::save(xml_document<>& doc, xml_node<>* nodeParent) 
{
    xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

    newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));
    newNode->append_attribute(doc.allocate_attribute("activationRadius", doc.allocate_string(std::to_string(_activationRadius).c_str())));
    nodeParent->append_node(newNode);
}