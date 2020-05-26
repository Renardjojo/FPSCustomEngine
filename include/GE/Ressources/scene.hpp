//Project : Engine
//Editing by Six Jonathan
//Date : 10/01/2020 - 16 h 44

#ifndef _GE_SCENE_H
#define _GE_SCENE_H

#include <vector>
#include <string>
#include "GE/Core/Maths/mat.hpp"
#include "GE/Core/DataStructure/graph.hpp"

namespace Engine::Ressources
{
    class Scene
        : public Engine::Core::DataStructure::Graph
    {
        public:

            #pragma region constructor/destructor

            Scene ()                    = default;
            Scene (const Scene& other)  = default;
            Scene (Scene&& other)       = default;
            ~Scene ()                   = default;

            #pragma endregion //!constructor/destructor

            #pragma region methods

            /**
             * @brief update each entity moved and these childen.
             * 
             */
            void update () noexcept
            {
                world_.updateSelfAndChild();
            }

            #pragma endregion //!methods

            /**
             * @brief Use this Scene to show
             * 
             */
            void use () noexcept
            {
                sceneToUse = this;
            }

            static Scene* getSceneUse() noexcept { return sceneToUse; }

        protected:

            static Scene* sceneToUse; //pointor to be in nullptr by default

        private:

    };
} //namespace Engine::Ressrouces

#endif // _GE_SCENE_H