//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-30 - 17 h 39

#ifndef _LIFE_LOOT_H
#define _LIFE_LOOT_H

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/Random.hpp"
#include "GE/Physics/PhysicSystem.hpp"
#include "GE/Ressources/scene.hpp"
#include "GE/LowRenderer/model.hpp"
#include "GE/Ressources/ressourcesManager.hpp"
#include "GE/Ressources/mesh.hpp"
#include "GE/Ressources/material.hpp"
#include "GE/Ressources/shader.hpp"

#include "Game/ParticuleGenerator.hpp"
#include "Game/Loot.hpp"
#include "Game/PlayerController.hpp"
#include "Game/LifeDuration.hpp"

#include <vector>

namespace Game
{
    class LifeLoot : public Loot
    {
        protected :

        public:

        LifeLoot(Engine::Ressources::GameObject &gameObject);

        LifeLoot (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params);

        virtual ~LifeLoot() = default;

        virtual void effect();
    };

} //namespace Game

#endif //_LIFE_LOOT_H