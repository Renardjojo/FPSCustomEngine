//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-08 - 17 h 39

#ifndef _CIRCULAR_ENEMIES_SPAWNER_H
#define _CIRCULAR_ENEMIES_SPAWNER_H

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
    typedef std::vector<std::string> EnemiePrefabs;


    class CircularEnemiesSpawner : public Engine::Core::Component::ScriptComponent
    {
        private:
        
        EnemiePrefabs                           _enemiePrefas          {};
        float                                   _zoneRadius            {3.f};  /*in sec*/
        float                                   _spawnDelay            {1.f};  /*in sec*/
        float                                   _spawnDelayInterval    {0.f}; /*in sec*/
        float                                   _delayCount            {0.f};
        float                                   _nextDelay             {_spawnDelay};

        
        public:

        /**
         * @brief Construct a new Ranged Enemies Spawner object
         * 
         * @param gameObject 
         * @param spawnPosition 
         * @param zoneRadius 
         * @param spawnDelay 
         * @param spawnDelayInterval : spawnDelay will be compute this + or - this intervale.  
         */
        CircularEnemiesSpawner(Engine::Ressources::GameObject &gameObject, const EnemiePrefabs& enemisPrefabs, Engine::Core::Maths::Vec3& spawnPosition, float zoneRadius, float spawnDelay, float spawnDelayInterval = 0.f)
            :   Engine::Core::Component::ScriptComponent    {gameObject},
                _enemiePrefas                               {enemisPrefabs},
                _zoneRadius                                 {zoneRadius},
                _spawnDelay                                 {spawnDelay},
                _spawnDelayInterval                         {spawnDelayInterval}, 
                _delayCount                                 {0.f},
                _nextDelay                                  {_spawnDelay + Engine::Core::Maths::Random::ranged(-_spawnDelayInterval, _spawnDelayInterval)}
        {}

        CircularEnemiesSpawner (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params)
            :   Engine::Core::Component::ScriptComponent    {refGameObject},
                _enemiePrefas                               {},
                _zoneRadius                                 {std::stof(params[0])},
                _spawnDelay                                 {std::stof(params[1])},
                _spawnDelayInterval                         {std::stof(params[2])}, 
                _delayCount                                 {std::stof(params[3])},
                _nextDelay                                  {std::stof(params[4])}
        {
            _name = __FUNCTION__;

            int count = 5;

            while (params.size() > count && params[count].substr(0, 2) == "#1")
            {
                _enemiePrefas.push_back(params[count].substr(2));
                count++;
            }
        }


        virtual ~CircularEnemiesSpawner() = default;

        void update() override
        {   
            if (_enemiePrefas.empty())
                return;

            _delayCount += Engine::Core::System::TimeSystem::getDeltaTime();

            while (_delayCount >= _nextDelay)
            {
                _delayCount -= _nextDelay;
                _nextDelay   = _spawnDelay + Engine::Core::Maths::Random::ranged(-_spawnDelayInterval, _spawnDelayInterval);
                Engine::Core::Maths::Vec3 newPosition = Engine::Core::Maths::Random::peripheralSphericalCoordinate(_gameObject.getGlobalPosition(), _zoneRadius);
                Engine::Ressources::Save::loadPrefab(_gameObject, newPosition, _enemiePrefas[Engine::Core::Maths::Random::ranged<int>(_enemiePrefas.size() - 1)]);
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
            for (auto &&path : _enemiePrefas)
            {
                std::string data = "#";
                data += std::to_string(1);
                data += path;
                newNode->append_attribute(doc.allocate_attribute((std::string("spawnerPrefabs") + std::to_string(count)).c_str(), doc.allocate_string(data.c_str())));
                count++;
            }

            nodeParent->append_node(newNode);
        }
    };

} //namespace Game

#endif //_CIRCULAR_ENEMIES_SPAWNER_H