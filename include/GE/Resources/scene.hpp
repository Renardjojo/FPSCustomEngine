//Project : Engine
//Editing by Six Jonathan
//Date : 10/01/2020 - 16 h 44

#ifndef _GE_SCENE_H
#define _GE_SCENE_H

#include <vector>
#include <string>
#include "GE/Core/Maths/mat.hpp"
#include "GE/Core/DataStructure/graph.hpp"

namespace Engine::Resources
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
             * @brief Update each entity moved and these childen.
             * 
             */
            void update () noexcept
            {
                world_.update();
            }

            void draw () const noexcept
            {
                std::map<float, Engine::LowRenderer::Model*> mapElemSortedByDistance;

                glDisable(GL_BLEND);
                world_.sortAndDrawOpqueElement(mapElemSortedByDistance);

                glEnable(GL_BLEND);
                for(std::map<float, Engine::LowRenderer::Model*>::reverse_iterator it = mapElemSortedByDistance.rbegin(); it != mapElemSortedByDistance.rend(); ++it) 
                {
                    it->second->draw();
                }
            }

            /**
             * @brief check light enable and store it to use them to draw element with shader light
             * 
             */
            void updateLigthToUse() noexcept;

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
            #pragma endregion //!attribut

            #pragma region static attribut
            #pragma endregion //! static attribut

            #pragma region methods
            #pragma endregion //!methods

        private:

    };
} //namespace Engine::Ressrouces

#endif // _GE_SCENE_H