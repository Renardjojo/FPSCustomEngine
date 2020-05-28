//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-08 - 17 h 39

#ifndef _CIRCULAR_ENTITIES_SPAWNER_H
#define _CIRCULAR_ENTITIES_SPAWNER_H

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/Random.hpp"
#include "GE/LowRenderer/model.hpp"
#include "GE/Ressources/Saves.hpp"
#include "Game/LifeDuration.hpp"
#include "GE/Physics/PhysicalObject.hpp"
#include "GE/Physics/ColliderShape/SphereCollider.hpp"

namespace Game
{
    struct EntityPrefabCount
    {
        unsigned int numberEntity;
        std::string  pathPrefabs;
    };

    class CircularEntitiesSpawner : public Engine::Core::Component::ScriptComponent
    {
        private:
        
        std::vector<EntityPrefabCount>          _entitiesToSpawnInfo   {};
        float                                   _zoneRadius            {3.f};  /*in sec*/
        float                                   _spawnDelay            {1.f};  /*in sec*/
        float                                   _spawnDelayInterval    {0.f}; /*in sec*/
        float                                   _delayCount            {0.f};
        float                                   _nextDelay             {_spawnDelay};

        
        public:

        /**
         * @brief Construct a new Ranged Entitiess Spawner object
         * 
         * @param gameObject 
         * @param spawnPosition 
         * @param zoneRadius 
         * @param spawnDelay 
         * @param spawnDelayInterval : spawnDelay will be compute this + or - this intervale.  
         */
        CircularEntitiesSpawner(Engine::Ressources::GameObject &gameObject, const std::vector<EntityPrefabCount>& entitiesToSpawnInfo, float zoneRadius, float spawnDelay, float spawnDelayInterval = 0.f)
            :   Engine::Core::Component::ScriptComponent    {gameObject},
                _entitiesToSpawnInfo                        {entitiesToSpawnInfo},
                _zoneRadius                                 {zoneRadius},
                _spawnDelay                                 {spawnDelay},
                _spawnDelayInterval                         {spawnDelayInterval}, 
                _delayCount                                 {0.f},
                _nextDelay                                  {_spawnDelay + Engine::Core::Maths::Random::ranged(-_spawnDelayInterval, _spawnDelayInterval)}
        {}

        CircularEntitiesSpawner(Engine::Ressources::GameObject &gameObject, float zoneRadius, float spawnDelay, float spawnDelayInterval = 0.f)
            :   Engine::Core::Component::ScriptComponent    {gameObject},
                _entitiesToSpawnInfo                        {},
                _zoneRadius                                 {zoneRadius},
                _spawnDelay                                 {spawnDelay},
                _spawnDelayInterval                         {spawnDelayInterval}, 
                _delayCount                                 {0.f},
                _nextDelay                                  {_spawnDelay + Engine::Core::Maths::Random::ranged(-_spawnDelayInterval, _spawnDelayInterval)}
        {}

        CircularEntitiesSpawner (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params)
            :   Engine::Core::Component::ScriptComponent    {refGameObject},
                _entitiesToSpawnInfo                        {},
                _zoneRadius                                 {std::stof(params[0])},
                _spawnDelay                                 {std::stof(params[1])},
                _spawnDelayInterval                         {std::stof(params[2])}, 
                _delayCount                                 {std::stof(params[3])},
                _nextDelay                                  {std::stof(params[4])}
        {
            _name = __FUNCTION__;

            size_t count = 5;

            while (params.size() > count)
            {
                _entitiesToSpawnInfo.push_back({static_cast<unsigned int>(std::stoi(params[count])), params[count + 1]});
                count+=2;
            }
        }

        void setEntitiesToSpawn(unsigned int numberEntities, const std::string& prefabs)
        {
            if (numberEntities > 0)
                _entitiesToSpawnInfo.push_back({numberEntities, prefabs});
        }

        virtual ~CircularEntitiesSpawner() = default;

        void update() override
        {  
            if (_entitiesToSpawnInfo.empty())
                return;

            _delayCount += Engine::Core::System::TimeSystem::getDeltaTime();

            while (_delayCount >= _nextDelay)
            {
                _delayCount -= _nextDelay;
                _nextDelay   = _spawnDelay + Engine::Core::Maths::Random::ranged(-_spawnDelayInterval, _spawnDelayInterval);
                Engine::Core::Maths::Vec3 newPosition = Engine::Core::Maths::Random::peripheralSphericalCoordinate(_gameObject.getGlobalPosition(), _zoneRadius);

                /*Choose random entity*/
                unsigned int indexEntityToSpawn = Engine::Core::Maths::Random::ranged<int>(_entitiesToSpawnInfo.size() - 1);

                /*Spawn this entity*/
                Engine::Ressources::Save::loadPrefab(_gameObject, newPosition, _entitiesToSpawnInfo[indexEntityToSpawn].pathPrefabs);

                /*Remove this entity. If all the entities of a type were generated, remove this entity's type*/
                _entitiesToSpawnInfo[indexEntityToSpawn].numberEntity--;
                if (_entitiesToSpawnInfo[indexEntityToSpawn].numberEntity == 0)
                {
                    _entitiesToSpawnInfo.erase(_entitiesToSpawnInfo.begin() + indexEntityToSpawn);
                }
            }
        }

        void save(xml_document<>& doc, xml_node<>* nodeParent) 
        {
            xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

            newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));

            newNode->append_attribute(doc.allocate_attribute("zoneRadius", doc.allocate_string(std::to_string(_zoneRadius).c_str())));
            newNode->append_attribute(doc.allocate_attribute("spawnDelay", doc.allocate_string(std::to_string(_spawnDelay).c_str())));
            newNode->append_attribute(doc.allocate_attribute("spawnDelayInterval", doc.allocate_string(std::to_string(_spawnDelayInterval).c_str())));
            newNode->append_attribute(doc.allocate_attribute("delayCount", doc.allocate_string(std::to_string(_delayCount).c_str())));
            newNode->append_attribute(doc.allocate_attribute("nextDelay", doc.allocate_string(std::to_string(_nextDelay).c_str())));

            int count = 0;
            for (auto &&i : _entitiesToSpawnInfo)
            {
                newNode->append_attribute(doc.allocate_attribute((std::string("numberEntity") + std::to_string(count)).c_str(), doc.allocate_string(std::to_string(i.numberEntity).c_str())));
                newNode->append_attribute(doc.allocate_attribute((std::string("pathPrefabs") + std::to_string(count)).c_str(), doc.allocate_string(i.pathPrefabs.c_str())));
                count++;
            }

            nodeParent->append_node(newNode);
        }
    };

} //namespace Game

#endif //_CIRCULAR_ENTITIES_SPAWNER_H