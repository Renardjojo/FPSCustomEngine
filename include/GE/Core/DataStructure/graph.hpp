//Project : Engine
//Editing by Six Jonathan
//Date : 13/01/2020 - 12 h 42

#ifndef _GE_GRAPH_H
#define _GE_GRAPH_H

#include <list>
#include <map>
#include <memory>
#include <string>
#include <istream>
#include <sstream>
#include <iostream>
#include "GE/LowRenderer/entity.hpp"
#include "GE/Core/Debug/log.hpp"
#include "GE/LowRenderer/model.hpp"
#include "GE/LowRenderer/camera.hpp"
#include "GE/Ressources/GameObject.hpp"

namespace Engine::Core::DataStructure
{
    class Graph
    {
        public:

            #pragma region constructor/destructor

            Graph ()
                : world_ {}
            {
                world_.setName("World");
            }

            Graph (const Graph& other)		= delete;
            Graph (Graph&& other)			= default;
            ~Graph ()				        = default;

            #pragma endregion //!constructor/destructor

            #pragma region methods

            #pragma endregion //!methods

            #pragma region accessor

            /**
             * @brief Get the World object
             * 
             * @return GraphEntity* 
             */
            Engine::Ressources::GameObject& getWorld() noexcept { return world_;}
            const Engine::Ressources::GameObject& getWorld() const noexcept { return world_;}

            /**
             * @brief Get the Entity object in function of path in arg
             * 
             * @param path : example world/car/motor/piston3 or car/motor/piston3 or ./car/motor/piston3 
             * @return GraphEntity& 
             */
            Engine::Ressources::GameObject& getGameObject (const std::string& path) noexcept
            {
                GE_assert(!path.empty());

                std::stringstream sPath(path);
                std::string word;
                Engine::Ressources::GameObject* currentEntity = &world_;

                while (std::getline(sPath, word, '/'))
                {
                    if (word.empty() || word == "." || word == "world") continue;

                    bool isFound = false;
                    for (auto&& child : currentEntity->children)
                    {
                        if (child->getName() == word)
                        {
                            currentEntity = child.get();
                            isFound = true;
                            break;
                        }
                    }
                    if (!isFound)
                    {
                        Engine::Core::Debug::SLog::logError(std::string("Canno't found \"") + word + "\" in scene graph \"" + path + "\"" );
                        exit(1);
                    }
                }
                return *currentEntity;
            }

            #pragma endregion //!accessor

            #pragma region mutator

            /**
             * @brief add specific entity to the graph with arg to construct it and return his id
             * 
             * @tparam T 
             * @tparam Args 
             * @param args 
             * @param dependenceEntity&
             * @return GameObject&
             */
            template<typename T, typename ...Args>
            Engine::Ressources::GameObject& add(Engine::Ressources::GameObject& dependenceEntity, Args&&... args) noexcept
            {
                dependenceEntity.children.emplace_back(std::make_unique<T>(args...));
                dependenceEntity.children.back()->children = std::list<std::unique_ptr<Engine::Ressources::GameObject>>();
                dependenceEntity.children.back()->update(dependenceEntity.getModelMatrix());

                return *dependenceEntity.children.back();
            }

            #pragma endregion //!mutator

            #pragma region operator
            #pragma endregion //!operator

            #pragma region convertor
            #pragma endregion //!convertor

        protected:

            #pragma region attribut

		    Engine::Ressources::GameObject world_;

            #pragma endregion //!attribut

        private:

    };
} //namespace Engine::Core::DataStructure

#endif // _GE_GRAPH_H