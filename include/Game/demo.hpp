//Project : Engine
//Editing by Six Jonathan
//Date : 29/01/2020 - 09 h 15

#ifndef _DEMO_H
#define _DEMO_H

#include "GE/Ressources/scene.hpp"
#include "GE/GE.hpp"
#include "GE/Ressources/ressourcesManager.hpp"
#include "GE/Core/InputSystem/input.hpp"
#include "GE/Ressources/mesh.hpp"
#include "GE/Ressources/shader.hpp"
#include "GE/LowRenderer/model.hpp"
#include "GE/Ressources/sample.hpp"
#include "GE/Ressources/material.hpp"
#include "GE/Ressources/music.hpp"
#include "GE/Ressources/font.hpp"
#include "GE/Ressources/ui.hpp"
#include "GE/Core/Maths/vec.hpp"

#include "GE/Physics/PhysicalObject.hpp"
#include "GE/Physics/PhysicSystem.hpp"

namespace Game
{
    class Demo
    {
        public:

            #pragma region constructor/destructor

            Demo (Engine::GE& gameEngine);
            
            Demo ()                         = delete;
            Demo (const Demo& other)		= default;
            Demo (Demo&& other)				= default;  
            virtual ~Demo ()				= default;

            #pragma endregion //!constructor/destructor

            #pragma region methods

            void update     () noexcept;
            void display    () const noexcept;

            #pragma endregion //!methods

        protected:

            #pragma region attribut

            Engine::GE& gameEngine_;
            std::unique_ptr<Engine::Ressources::Scene> scene_;

            bool flagleftClicIsDown;
            bool flagF1IsDown;
            bool usingMouse;

            Engine::Ressources::GameObject* sunLight;
            /*Engine::Core::Maths::Vec3 dirPlayer1;
            Engine::Ressources::GameObject* player1;
            Engine::Ressources::GameObject* player2;*/

            float testLifePLayer = 100.f;

            Engine::Core::Maths::Vec3 dirCamera;
            Engine::Ressources::GameObject* mainCamera;

            Engine::Physics::PhysicalObject* physictest;
            #pragma endregion //!attribut

            #pragma region methods

            void loadRessources         (Engine::Ressources::t_RessourcesManager& ressourceManager);
            void loadEntity             (Engine::Ressources::t_RessourcesManager& ressourceManager);
            void loadLights             (Engine::Ressources::t_RessourcesManager& ressourceManager);
            void loadUI                 (Engine::Ressources::t_RessourcesManager& ressourceManager);
            void loadCamera             (Engine::Ressources::t_RessourcesManager& ressourceManager);
            // void loadPlayers            (Ressources& ressourceManager);
            /*void loadSound              (Ressources& ressourceManager);
            void loadSkyBox             (Ressources& ressourceManager);
            void loadGround             (Ressources& ressourceManager);
            void loadMultipleRock       (Ressources& ressourceManager);
            void loadMultipleCrash      (Ressources& ressourceManager);
            void loadTowerGuard         (Ressources& ressourceManager);
            void loadTree               (Ressources& ressourceManager);
            void loadCloud              (Ressources& ressourceManager);*/

            void updateControl          (Engine::Core::InputSystem::Input& input);

            #pragma endregion //!methods

        private:

    };
}

#endif // _DEMO_H
