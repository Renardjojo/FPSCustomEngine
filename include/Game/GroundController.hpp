//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-22 - 17 h 39

#ifndef _GROUND_CONTROLLER_H
#define _GROUND_CONTROLLER_H

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Ressources/GameObject.hpp"

#include "GE/Physics/ColliderShape/Collider.hpp"

namespace Game
{
    class GroundController : public Engine::Core::Component::ScriptComponent
    {
        private:


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
        GroundController(Engine::Ressources::GameObject &gameObject);

        virtual ~GroundController() = default;

        void onCollisionEnter (Engine::Physics::ColliderShape::HitInfo& hitInfo) override;

        void save(xml_document<>& doc, xml_node<>* nodeParent);
    };

} //namespace Game

#endif //_GROUND_CONTROLLER_H