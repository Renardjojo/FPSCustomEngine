#include "Game/CircularEntitiesSpawner.hpp"
#include "GE/Ressources/scene.hpp"

using namespace Game;
using namespace Engine::Ressources;
using namespace Engine::Core::Component;
using namespace Engine::Core::Maths;

CircularEntitiesSpawner::CircularEntitiesSpawner(GameObject &gameObject, GameObject* contenor, const std::vector<EntityPrefabCount>& entitiesToSpawnInfo, float zoneRadius, float spawnDelay, float spawnDelayInterval)
    :   ScriptComponent    {gameObject},
        _entitiesToSpawnInfo                        {entitiesToSpawnInfo},
        _zoneRadius                                 {zoneRadius},
        _spawnDelay                                 {spawnDelay},
        _spawnDelayInterval                         {spawnDelayInterval}, 
        _delayCount                                 {0.f},
        _nextDelay                                  {_spawnDelay + Random::ranged(-_spawnDelayInterval, _spawnDelayInterval)},
        _contenor                                   {contenor}
{
    _name = __FUNCTION__;
}

CircularEntitiesSpawner::CircularEntitiesSpawner(GameObject &gameObject, GameObject* contenor, float zoneRadius, float spawnDelay, float spawnDelayInterval)
    :   ScriptComponent    {gameObject},
        _entitiesToSpawnInfo                        {},
        _zoneRadius                                 {zoneRadius},
        _spawnDelay                                 {spawnDelay},
        _spawnDelayInterval                         {spawnDelayInterval}, 
        _delayCount                                 {0.f},
        _nextDelay                                  {_spawnDelay + Random::ranged(-_spawnDelayInterval, _spawnDelayInterval)},
        _contenor                                   {contenor}
{
    _name = __FUNCTION__;
}

CircularEntitiesSpawner::CircularEntitiesSpawner (GameObject &refGameObject, const std::vector<std::string>& params)
    :   ScriptComponent    {refGameObject},
        _entitiesToSpawnInfo                        {},
        _zoneRadius                                 {std::stof(params[0])},
        _spawnDelay                                 {std::stof(params[1])},
        _spawnDelayInterval                         {std::stof(params[2])}, 
        _delayCount                                 {std::stof(params[3])},
        _nextDelay                                  {std::stof(params[4])},
        _contenor                                   {&Scene::getCurrentScene()->getGameObject(params[5])}
{
    _name = __FUNCTION__;

    size_t count = 6;

    while (params.size() > count)
    {
        _entitiesToSpawnInfo.push_back({static_cast<unsigned int>(std::stoi(params[count])), params[count + 1]});
        count+=2;
    }
}

void CircularEntitiesSpawner::addEntitiesToSpawner(unsigned int numberEntities, const std::string& prefabs)
{
    if (numberEntities > 0)
        _entitiesToSpawnInfo.push_back({numberEntities, prefabs});
}

void CircularEntitiesSpawner::update()
{  
    if (_entitiesToSpawnInfo.empty())
        return;

    _delayCount += Engine::Core::System::TimeSystem::getDeltaTime();

    while (_delayCount >= _nextDelay)
    {
        _delayCount -= _nextDelay;
        _nextDelay   = _spawnDelay + Random::ranged(-_spawnDelayInterval, _spawnDelayInterval);
        Vec2 position2D = Random::circularCoordinate({_gameObject.getGlobalPosition().x, _gameObject.getGlobalPosition().z}, _zoneRadius);
        Vec3 newPosition = {position2D.x, _gameObject.getGlobalPosition().y ,position2D.y};

        /*Choose random entity*/
        unsigned int indexEntityToSpawn = Random::ranged<int>(_entitiesToSpawnInfo.size() - 1);

        /*Spawn this entity*/
        Save::loadPrefab( _contenor == nullptr ? _gameObject : *_contenor, newPosition, _entitiesToSpawnInfo[indexEntityToSpawn].pathPrefabs);

        /*Remove this entity. If all the entities of a type were generated, remove this entity's type*/
        _entitiesToSpawnInfo[indexEntityToSpawn].numberEntity--;
        if (_entitiesToSpawnInfo[indexEntityToSpawn].numberEntity == 0)
        {
            _entitiesToSpawnInfo.erase(_entitiesToSpawnInfo.begin() + indexEntityToSpawn);
        }
    }
}

void CircularEntitiesSpawner::save(xml_document<>& doc, xml_node<>* nodeParent) 
{
    xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

    newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));

    newNode->append_attribute(doc.allocate_attribute("zoneRadius", doc.allocate_string(std::to_string(_zoneRadius).c_str())));
    newNode->append_attribute(doc.allocate_attribute("spawnDelay", doc.allocate_string(std::to_string(_spawnDelay).c_str())));
    newNode->append_attribute(doc.allocate_attribute("spawnDelayInterval", doc.allocate_string(std::to_string(_spawnDelayInterval).c_str())));
    newNode->append_attribute(doc.allocate_attribute("delayCount", doc.allocate_string(std::to_string(_delayCount).c_str())));
    newNode->append_attribute(doc.allocate_attribute("nextDelay", doc.allocate_string(std::to_string(_nextDelay).c_str())));

    newNode->append_attribute(doc.allocate_attribute("contenorName", doc.allocate_string(_contenor->getRelativePath().c_str())));

    int count = 0;
    for (auto &&i : _entitiesToSpawnInfo)
    {
        newNode->append_attribute(doc.allocate_attribute((std::string("numberEntity") + std::to_string(count)).c_str(), doc.allocate_string(std::to_string(i.numberEntity).c_str())));
        newNode->append_attribute(doc.allocate_attribute((std::string("pathPrefabs") + std::to_string(count)).c_str(), doc.allocate_string(i.pathPrefabs.c_str())));
        count++;
    }

    nodeParent->append_node(newNode);
}