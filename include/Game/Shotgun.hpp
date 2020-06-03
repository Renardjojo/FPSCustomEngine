//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-30 - 17 h 39

#ifndef _SHOTGUN_H
#define _SHOTGUN_H

#include "Game/Firearm.hpp"

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/Random.hpp"
#include "GE/Physics/PhysicSystem.hpp"
#include "GE/Ressources/Sound.hpp"
#include "GE/Ressources/SoundPlayer.hpp"


namespace Game
{
    class Shotgun : public Firearm
    {
        protected:

        float _radiusPrescisionAtOnMeter;

        public:

        Shotgun(Engine::Ressources::GameObject &gameObject, float bulletDamage, float bulletVelocity, unsigned int bulletPerShot, float reloadTime, unsigned int munitionCapacity, float shotIntervalDelay, float radiusPrescisionAtOnMeter,Engine::Ressources::Sound* sound)
            :   Firearm {gameObject, bulletDamage, bulletVelocity, bulletPerShot, reloadTime, munitionCapacity, shotIntervalDelay, false,sound},
                _radiusPrescisionAtOnMeter {radiusPrescisionAtOnMeter}
        {
            _name = __FUNCTION__;
        }

        Shotgun (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params)
            :   Firearm {refGameObject, params},
                _radiusPrescisionAtOnMeter{std::stof(params[11])}
        {
            _name = __FUNCTION__;
        }

        virtual ~Shotgun() = default;

        void shoot (const Engine::Core::Maths::Vec3& startPoint, const Engine::Core::Maths::Vec3& direction) noexcept override
        {
            if (_isReloading || _isWaitingForNextShot)
            {
                return;
            }

            /*Auto reload*/
            if (_munition == 0)
            {
                reload();
                return;
            }
            
            if(_sound)
                Engine::Ressources::SoundPlayer::play(*_sound);

            _munition -= _bulletPerShot;
            _isWaitingForNextShot = true;
            
            Engine::Physics::ColliderShape::HitInfo rayInfo;

            for (size_t i = 0; i < _bulletPerShot; i++)
            {
                Engine::Core::Maths::Vec3 newDir = direction + Engine::Core::Maths::Random::sphericalCoordinate(direction.getNormalize(), _radiusPrescisionAtOnMeter);

                //TODO: remove direction * 6.f when layer integrate the player (avoid to shoot the player)
                if (Engine::Physics::PhysicSystem::rayCast(startPoint + direction * 6.f, newDir, 10000.f, rayInfo))
                {
                    Engine::Physics::ColliderShape::Collider* pCollider = rayInfo.gameObject->getComponent<Engine::Physics::ColliderShape::Collider>();
                    Engine::Ressources::GameObject tempGOWithTag;
                    tempGOWithTag.setTag("Bullet");
                    Engine::Physics::ColliderShape::HitInfo hitInfo1 {rayInfo.intersectionsInfo, &tempGOWithTag, _bulletVelocity};
                    pCollider->OnCollisionEnter(hitInfo1);
                }
            }
            

        }

        virtual void aim () noexcept
        {

        }

        virtual void save(xml_document<>& doc, xml_node<>* nodeParent) 
        {
            xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

            newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));
            newNode->append_attribute(doc.allocate_attribute("bulletDamage", doc.allocate_string(std::to_string(_bulletDamage).c_str())));
            newNode->append_attribute(doc.allocate_attribute("bulletVelocity", doc.allocate_string(std::to_string(_bulletVelocity).c_str())));
            newNode->append_attribute(doc.allocate_attribute("bulletPerShot", doc.allocate_string(std::to_string(_bulletPerShot).c_str())));
            newNode->append_attribute(doc.allocate_attribute("reloadTime", doc.allocate_string(std::to_string(_reloadTime).c_str())));
            newNode->append_attribute(doc.allocate_attribute("munitionCapacity", doc.allocate_string(std::to_string(_munitionCapacity).c_str())));
            newNode->append_attribute(doc.allocate_attribute("shotIntervalDelay", doc.allocate_string(std::to_string(_shotIntervalDelay).c_str())));
            newNode->append_attribute(doc.allocate_attribute("munition", doc.allocate_string(std::to_string(_munition).c_str())));
            newNode->append_attribute(doc.allocate_attribute("delay", doc.allocate_string(std::to_string(_delay).c_str())));
            newNode->append_attribute(doc.allocate_attribute("isReloading", doc.allocate_string(std::to_string(_isReloading).c_str())));
            newNode->append_attribute(doc.allocate_attribute("isWaitingForNextShot", doc.allocate_string(std::to_string(_isWaitingForNextShot).c_str())));
            newNode->append_attribute(doc.allocate_attribute("isAutomatic", doc.allocate_string(std::to_string(_isAutomatic).c_str())));
            newNode->append_attribute(doc.allocate_attribute("radiusPrescisionAtOnMeter", doc.allocate_string(std::to_string(_radiusPrescisionAtOnMeter).c_str())));

            nodeParent->append_node(newNode);
        }
    };

} //namespace Game

#endif //_SHOTGUN_H