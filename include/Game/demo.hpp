//Project : Engine
//Editing by Six Jonathan
//Date : 29/01/2020 - 09 h 15

#ifndef _DEMO_H
#define _DEMO_H

#include "scene.hpp"
#include "GE.hpp"
#include "resourcesManager.hpp"
#include "input.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "model.hpp"
#include "sample.hpp"
#include "material.hpp"
#include "music.hpp"
#include "font.hpp"
#include "vec.hpp"

namespace Game
{

    typedef Engine::Resources::ResourcesManager<Engine::Resources::Mesh, Engine::Resources::Shader, Engine::Resources::Material, Engine::Resources::Sample, Engine::Resources::Music, Engine::Resources::Font> Resources;

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
            Engine::Resources::Scene scene_;

            bool flagleftClicIsDown;
            bool flagF1IsDown;
            bool mouseForPlayer1;

            /*Engine::Core::Maths::Vec3 dirPlayer1;
            Engine::Core::Maths::Vec3 dirPlayer2;
            Engine::Core::DataStructure::GraphEntity* player1;
            Engine::Core::DataStructure::GraphEntity* player2;
            Engine::Core::DataStructure::GraphEntity* sunLight;*/

            Engine::Core::DataStructure::GraphEntity* mainCamera;


            #pragma endregion //!attribut

            #pragma region static attribut
            
            #pragma endregion //! static attribut

            #pragma region methods

            void loadResourceAndScene  ();

            void loadGeneralRessource   (Resources& resourceManager);
            /*void loadSound              (Resources& resourceManager);
            void loadPlayers            (Resources& resourceManager, const Engine::Resources::Size& windowSize);
            void loadSkyBox             (Resources& resourceManager);
            void loadGround             (Resources& resourceManager);
            void loadMultipleRock       (Resources& resourceManager);
            void loadMultipleCrash      (Resources& resourceManager);
            void loadLights             (Resources& resourceManager);
            void loadTowerGuard         (Resources& resourceManager);
            void loadTree               (Resources& resourceManager);
            void loadCloud              (Resources& resourceManager);*/

            void updateControl          (const Engine::Core::InputSystem::Input& input, float detlaTime);

            #pragma endregion //!methods

        private:

    };
}

#endif // _DEMO_H
