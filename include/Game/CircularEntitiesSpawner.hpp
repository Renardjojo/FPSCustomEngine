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
#include "Game/Checkpoint.hpp"
#include "GE/Physics/PhysicalObject.hpp"
#include "GE/Physics/ColliderShape/SphereCollider.hpp"
#include "GE/Ressources/scene.hpp"

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
        
        std::vector<EntityPrefabCount>          _entitiesToSpawnInfo    {};
        float                                   _zoneRadius             {3.f};  /*in sec*/
        float                                   _spawnDelay             {1.f};  /*in sec*/
        float                                   _spawnDelayInterval     {0.f}; /*in sec*/
        float                                   _delayCount             {0.f};
        float                                   _nextDelay              {_spawnDelay};
        Engine::Ressources::GameObject*         _contenor               {nullptr};
        Game::Checkpoint*                       _checkpoint             {nullptr};
        
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
        CircularEntitiesSpawner(Engine::Ressources::GameObject &gameObject, Engine::Ressources::GameObject* contenor, const std::vector<EntityPrefabCount>& entitiesToSpawnInfo, float zoneRadius, float spawnDelay, float spawnDelayInterval = 0.f);

        CircularEntitiesSpawner(Engine::Ressources::GameObject &gameObject, Engine::Ressources::GameObject* contenor, Game::Checkpoint* checkpoint, float zoneRadius, float spawnDelay, float spawnDelayInterval = 0.f);

        CircularEntitiesSpawner (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params);

        void addEntitiesToSpawner(unsigned int numberEntities, const std::string& prefabs);

        virtual ~CircularEntitiesSpawner() = default;

        /**
         * @brief Return true if the spawn stop to spawn entities
         * 
         * @return true 
         * @return false 
         */
        bool isEmpty() const noexcept { return _entitiesToSpawnInfo.empty();}

        void update() override;

        void save(xml_document<>& doc, xml_node<>* nodeParent);
    };

} //namespace Game

#endif //_CIRCULAR_ENTITIES_SPAWNER_H