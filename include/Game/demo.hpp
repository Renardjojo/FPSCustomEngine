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
#include "GE/Core/Maths/vec.hpp"

#include "GE/Physics/PhysicalObject.hpp"
#include "GE/Physics/PhysicSystem.hpp"

namespace Game
{

    typedef Engine::Ressources::RessourcesManager<Engine::Ressources::Mesh, Engine::Ressources::Shader, Engine::Ressources::Material, Engine::Ressources::Sample, Engine::Ressources::Music, Engine::Ressources::Font> Ressources;

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

            #pragma region static methods
            #pragma endregion //!static methods

            #pragma region accessor
            #pragma endregion //!accessor

            #pragma region mutator
            #pragma endregion //!mutator

            #pragma region operator
            #pragma endregion //!operator

            #pragma region convertor
            #pragma endregion //!convertor

        protected:

            #pragma region attribut

            Engine::GE& gameEngine_;
            Engine::Ressources::Scene scene_;

            bool flagleftClicIsDown;
            bool flagF1IsDown;
            bool mouseForPlayer1;

            Engine::Ressources::GameObject* sunLight;
            /*Engine::Core::Maths::Vec3 dirPlayer1;
            Engine::Ressources::GameObject* player1;
            Engine::Ressources::GameObject* player2;*/

            Engine::Core::Maths::Vec3 dirCamera;
            Engine::Ressources::GameObject* mainCamera;

            Engine::Physics::PhysicalObject* physictest;
            #pragma endregion //!attribut

            #pragma region static attribut
            
            #pragma endregion //! static attribut

            #pragma region methods

            void loadRessourceAndScene  ();

            void loadGeneralRessource   (Ressources& ressourceManager);
            void loadLights             (Ressources& ressourceManager);
            /*void loadSound              (Ressources& ressourceManager);
            void loadPlayers            (Ressources& ressourceManager, const Engine::Ressources::Size& windowSize);
            void loadSkyBox             (Ressources& ressourceManager);
            void loadGround             (Ressources& ressourceManager);
            void loadMultipleRock       (Ressources& ressourceManager);
            void loadMultipleCrash      (Ressources& ressourceManager);
            void loadTowerGuard         (Ressources& ressourceManager);
            void loadTree               (Ressources& ressourceManager);
            void loadCloud              (Ressources& ressourceManager);*/

            void updateControl          (const Engine::Core::InputSystem::Input& input);

            #pragma endregion //!methods

        private:

    };
}

#endif // _DEMO_H
