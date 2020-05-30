//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-30 - 17 h 39

#ifndef _SNIPER_H
#define _SNIPER_H

#include "Game/FireGun.hpp"

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Physics/PhysicSystem.hpp"

namespace Game
{
    class Sniper : public FireGun
    {
        private:

        public:

        Sniper(Engine::Ressources::GameObject &gameObject, float bulletDamage, float bulletVelocity, unsigned int bulletPerShot, float realoadTime, unsigned int munitionCapacity, float shotIntervalDelay)
            :   FireGun {gameObject, bulletDamage, bulletVelocity, bulletPerShot, realoadTime, munitionCapacity, shotIntervalDelay, false}   
        {
            _name = __FUNCTION__;
        }

        Sniper (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params)
            :   FireGun {refGameObject, params} 
        {
            _name = __FUNCTION__;
        }

        virtual ~Sniper() = default;

        virtual void aim () noexcept
        {

        }
    };

} //namespace Game

#endif //_SNIPER_H