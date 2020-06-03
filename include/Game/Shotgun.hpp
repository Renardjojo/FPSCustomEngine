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

        Shotgun(Engine::Ressources::GameObject &gameObject, float bulletDamage, float bulletVelocity, unsigned int bulletPerShot, float reloadTime, unsigned int munitionCapacity, float shotIntervalDelay, float radiusPrescisionAtOnMeter,Engine::Ressources::Sound* sound);

        Shotgun (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params);

        virtual ~Shotgun() = default;

        void shoot (const Engine::Core::Maths::Vec3& startPoint, const Engine::Core::Maths::Vec3& direction) noexcept override;

        virtual void save(xml_document<>& doc, xml_node<>* nodeParent);
    };

} //namespace Game

#endif //_SHOTGUN_H