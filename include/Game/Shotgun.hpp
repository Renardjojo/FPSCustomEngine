//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-30 - 17 h 39

#ifndef _SHOTGUN_H
#define _SHOTGUN_H

#include "Game/FireGun.hpp"

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/Random.hpp"
#include "GE/Physics/PhysicSystem.hpp"

namespace Game
{
    class Shotgun : public FireGun
    {
        protected:

        float _radiusPrescisionAtOnMeter;

        public:

        Shotgun(Engine::Ressources::GameObject &gameObject, float bulletDamage, float bulletVelocity, unsigned int bulletPerShot, float realoadTime, unsigned int munitionCapacity, float shotIntervalDelay, float radiusPrescisionAtOnMeter)
            :   FireGun {gameObject, bulletDamage, bulletVelocity, bulletPerShot, realoadTime, munitionCapacity, shotIntervalDelay, false},
                _radiusPrescisionAtOnMeter {radiusPrescisionAtOnMeter}
        {
            _name = __FUNCTION__;
        }

        Shotgun (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params)
            :   FireGun {refGameObject, params},
                _radiusPrescisionAtOnMeter{} //TODO:
        {
            _name = __FUNCTION__;
        }

        virtual ~Shotgun() = default;

        void shoot (const Engine::Core::Maths::Vec3& startPoint, const Engine::Core::Maths::Vec3& direction) noexcept override
        {
            if (_isRelaoading || _isWaitingForNextShot)
            {
                return;
            }

            /*Auto reload*/
            if (_munition == 0)
            {
                reload();
                return;
            }

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

                    if (rayInfo.gameObject->getTag() != "Ground")
                        rayInfo.gameObject->destroy();   
                }
            }
            

        }

        virtual void aim () noexcept
        {

        }
    };

} //namespace Game

#endif //_SHOTGUN_H