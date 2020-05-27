//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-22 - 17 h 39

#ifndef _PUSHED_ON_SHOOT_H
#define _PUSHED_ON_SHOOT_H

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Physics/PhysicalObject.hpp"
#include "GE/Physics/ColliderShape/Collider.hpp"
#include "GE/Core/Debug/assert.hpp"

namespace Game
{
    class PushedOnShoot : public Engine::Core::Component::ScriptComponent
    {
        private:

        Engine::Physics::PhysicalObject*              _attachedPhysicalObject {nullptr};

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
        PushedOnShoot(Engine::Ressources::GameObject &gameObject)
            :   Engine::Core::Component::ScriptComponent    {gameObject},
                _attachedPhysicalObject                     {nullptr}
        {
            _name = __FUNCTION__;
        }

        virtual ~PushedOnShoot() = default;

        void start() override
        {
            _attachedPhysicalObject = _gameObject.getComponent<Engine::Physics::PhysicalObject>();
        }

        void onCollisionEnter (Engine::Physics::ColliderShape::HitInfo& hitInfo) override
        {
            if (hitInfo.gameObject->getTag() == "Bullet")
            {
                GE_assertInfo(_attachedPhysicalObject != nullptr, "Physical gameObject missing on dependant script");
                _attachedPhysicalObject->addForceAtPoint(hitInfo.intersectionsInfo.intersection1, -hitInfo.intersectionsInfo.normalI1 * 100.f);
            }
        }

        void save(xml_document<>& doc, xml_node<>* nodeParent)
        {
            if (!nodeParent)
                return;

            xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");
            newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));

            

            nodeParent->append_node(newNode);   
        }
    };

} //namespace Game

#endif //_PUSHED_ON_SHOOT_H