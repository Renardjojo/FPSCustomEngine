//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-30 - 17 h 39

#ifndef _SNIPER_H
#define _SNIPER_H

#include "Game/Firearm.hpp"
#include "Game/PlayerController.hpp"

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Physics/PhysicSystem.hpp"
#include "GE/Ressources/ressourcesManager.hpp"
#include "GE/Ressources/Sound.hpp"
#include "GE/Ressources/SoundPlayer.hpp"
#include "GE/Ressources/ui.hpp"
#include "GE/LowRenderer/model.hpp"
#include "GE/LowRenderer/camera.hpp"
#include "GE/Ressources/scene.hpp"

namespace Game
{
    class Sniper : public Firearm
    {
        private:

        public:

        Sniper(Engine::Ressources::GameObject &gameObject, float bulletDamage, float bulletVelocity, unsigned int bulletPerShot, float reloadTime, unsigned int munitionCapacity, float shotIntervalDelay,Engine::Ressources::Sound* sound);

        Sniper (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params);

        virtual ~Sniper() = default;

        virtual void switchAimState () noexcept;
    };

} //namespace Game

#endif //_SNIPER_H