//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-30 - 17 h 39

#ifndef _SUB_MACHINE_GUN_H
#define _SUB_MACHINE_GUN_H

#include "Game/Firearm.hpp"

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Physics/PhysicSystem.hpp"
#include "GE/Ressources/Sound.hpp"
#include "GE/Ressources/SoundPlayer.hpp"

namespace Game
{
    class SubMachineGun : public Firearm
    {
        private:

        public:

        SubMachineGun(Engine::Ressources::GameObject &gameObject, float bulletDamage, float bulletVelocity, unsigned int bulletPerShot, float reloadTime, unsigned int munitionCapacity, float shotIntervalDelay,Engine::Ressources::Sound* sound)
            :   Firearm {gameObject, bulletDamage, bulletVelocity, bulletPerShot, reloadTime, munitionCapacity, shotIntervalDelay, true,sound}   
        {
            _name = __FUNCTION__;
        }

        SubMachineGun (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params)
            :   Firearm {refGameObject, params} 
        {
            _name = __FUNCTION__;
        }

        virtual ~SubMachineGun() = default;

        virtual void aim () noexcept
        {

        }
    };

} //namespace Game

#endif //_SUB_MACHINE_GUN_H