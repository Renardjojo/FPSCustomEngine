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

namespace Engine::Core::DataStructure
{
    typedef struct S_GraphEntity
    {
        std::unique_ptr<Engine::LowRenderer::Entity>    entity;
        std::list<struct S_GraphEntity>                 children;

        /**
         * @brief update entity and these child if current entity is dirty
         * 
         */
        void update()
        {
            for (auto i = children.begin(); i != children.end(); i++)
            {
                if (i->entity->isDirty())
                {
                    i->entity->update(entity->getModelMatrix());
                    i->forceUpdate();
                }
                else
                {
                    i->update();
                }
            }      
        }

        /**
         * @brief Force the update of entity without check if entity is dirty
         * 
         */
        void forceUpdate()
        {
            for (auto i = children.begin(); i != children.end(); i++)
            {
                i->entity->update(entity->getModelMatrix());
                i->forceUpdate();
            }   
        }

        /**
         * @brief Draw entity if entity is drawable
         * 
         */
        void sortAndDrawOpqueElement(std::map<float, Engine::LowRenderer::Model*>& mapElemSortedByDistance) const
        {
            for (auto i = children.begin(); i != children.end(); i++)
            {
                Engine::LowRenderer::Model* model = dynamic_cast<Engine::LowRenderer::Model*>(i->entity.get());
                if (model != nullptr)
                {
                    if(model->isOpaque())
                        model->draw();
                    else
                    {
                        float distance = (Engine::LowRenderer::Camera::getCamUse()->getPosition() - model->getPosition()).length();
                        mapElemSortedByDistance[distance] = model;
                    }
                }

                i->sortAndDrawOpqueElement(mapElemSortedByDistance);
            }
        }    


    } GraphEntity;

    class Graph
    {
        public:

            #pragma region constructor/destructor

            Graph ()
            {
                Engine::LowRenderer::Entity world ( {0.f, 0.f, 0.f},
                                                    {0.f, 0.f, 0.f},
                                                    {1.f, 1.f, 1.f},
                                                    "World");

                world_.entity = std::make_unique<Engine::LowRenderer::Entity>(std::move(world));
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
            GraphEntity& getWorld() noexcept { return world_;}

            /**
             * @brief Get the Entity object in function of path in arg
             * 
             * @param path : example world/car/motor/piston3 or car/motor/piston3 or ./car/motor/piston3 
             * @return GraphEntity& 
             */
            GraphEntity& getEntity (const std::string& path) noexcept
            {
                GE_assert(!path.empty());

                std::stringstream sPath(path);
                std::string word;
                GraphEntity* currentEntity = &world_;

                while (std::getline(sPath, word, '/'))
                {
                    if (word.empty() || word == "." || word == "world") continue;

                    bool isFound = false;
                    for (auto&& child : currentEntity->children)
                    {
                        if (child.entity->getName() == word)
                        {
                            currentEntity = &child;
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
             * @return GraphEntity&
             */
            template<typename T, typename ...Args>
            GraphEntity& add(GraphEntity& dependenceEntity, Args&&... args) noexcept
            {
                dependenceEntity.children.push_back({});
                dependenceEntity.children.back().entity = std::make_unique<T>(args...);
                dependenceEntity.children.back().children = std::list<struct S_GraphEntity>();
                dependenceEntity.children.back().entity->update(dependenceEntity.entity->getModelMatrix());

                return dependenceEntity.children.back();
            }

            #pragma endregion //!mutator

            #pragma region operator
            #pragma endregion //!operator

            #pragma region convertor
            #pragma endregion //!convertor

        protected:

            #pragma region attribut

		    GraphEntity world_;

            #pragma endregion //!attribut

        private:

    };
} //namespace Engine::Core::DataStructure

#endif // _GE_GRAPH_H