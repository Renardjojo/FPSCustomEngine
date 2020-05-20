//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-29 - 19 h 27

#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <list>
#include <vector>
#include <string>
#include <istream>
#include <sstream>
#include <iostream>
#include <memory>
#include "GE/LowRenderer/entity.hpp"
#include "GE/Core/Debug/log.hpp"
#include "GE/Ressources/Component.hpp"

namespace Engine::Ressources
{
    typedef Engine::LowRenderer::EntityCreateArg GameObjectCreateArg;

    class GameObject : public Engine::LowRenderer::Entity
    {
    public:

        GameObject(const GameObjectCreateArg& arg)
            :  Engine::LowRenderer::Entity {arg}
        {}

        GameObject()                                        = default;
        GameObject(const GameObject &other)                 = default;
        GameObject(GameObject &&other)                      = default;
        virtual ~GameObject()                               = default;
        GameObject &operator=(GameObject const &other)      = default;
        GameObject &operator=(GameObject &&other)           = default;

        std::list<std::unique_ptr<GameObject>> children;

        /**
         * @brief update entity and these child if current entity is dirty
         * 
         */
        virtual void updateSelfAndChild() noexcept
        {
            for (auto&& i = children.begin(); i != children.end(); i++)
            {
                if ((*i)->isDirty())
                {
                    (*i)->update(modelMat_);
                    (*i)->forceUpdate();
                }
                else
                {
                    (*i)->updateSelfAndChild();
                }
            }
        }

        /**
         * @brief Force the update of entity without check if entity is dirty
         * 
         */
        void forceUpdate()
        {
            for (auto&& i = children.begin(); i != children.end(); i++)
            {
                (*i)->update(modelMat_);
                (*i)->forceUpdate();
            }
        }

        /**
         * @brief add a Component to the gameobject
         * 
         * @tparam T 
         * @tparam Args 
         * @param args 
         */
        template <typename T, typename... Args>
        T& addComponent(Args &&... args) noexcept
        {
            components.emplace_back(std::make_unique<T>(*this, args...));
            return *dynamic_cast<T*>(components.back().get());
        }

        /**
         * @brief Get the first Component type object
         * 
         * @tparam T 
         * @return Component& 
         */
        template <typename T>
        T* getComponent()
        {
            for (std::unique_ptr<Component> &uniquePtrComponent : components)
            {
                T* comp = dynamic_cast<T*>(uniquePtrComponent.get());

                if (comp != nullptr)
                {
                    return comp;
                }
            }
            return nullptr;
        }

        /**
         * @brief Get the first gameObject with path in arg
         * 
         * @param path : example world/car/motor/piston3 or car/motor/piston3 or ./car/motor/piston3 
         * @return GraphEntity& 
         */
        Engine::Ressources::GameObject* getChild (const std::string& path) noexcept
        {
            GE_assert(!path.empty());

            std::stringstream sPath(path);
            std::string word;
            Engine::Ressources::GameObject* currentEntity = this;

            while (std::getline(sPath, word, '/'))
            {
                if (word.empty() || word == "." || word == name_) continue;

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
                    Engine::Core::Debug::SLog::logWarning(std::string("Canno't found \"") + word + "\" in gameObject \"" + name_ + "\"" + " with path : \"" + path + "\"" );
                    return nullptr;
                }
            }
            return currentEntity;
        }

        /**
         * @brief Destroye the first gameObject with path in arg
         * 
         * @param path : example world/car/motor/piston3 or car/motor/piston3 or ./car/motor/piston3 
         * @return GraphEntity& 
         */
        // void destroyChild (const std::string& path) noexcept
        // {
        //     GE_assert(!path.empty());

        //     std::stringstream sPath(path);
        //     std::string word;
        //     Engine::Ressources::GameObject* parentEntity = this;
        //     Engine::Ressources::GameObject* currentEntity = this;

        //     while (std::getline(sPath, word, '/'))
        //     {
        //         if (word.empty() || word == "." || word == name_) continue;

        //         bool isFound = false;
        //         parentEntity = currentEntity;
        //         for (auto&& child : parentEntity->children)
        //         {
        //             if (child->getName() == word)
        //             {
        //                 currentEntity = child.get();
        //                 isFound = true;
        //                 break;
        //             }
        //         }
        //         if (!isFound)
        //         {
        //             Engine::Core::Debug::SLog::logWarning(std::string("Canno't found \"") + word + "\" in gameObject \"" + name_ + "\"" + " with path : \"" + path + "\"" );
        //             return;
        //         }
        //     }
        //     parentEntity->children.erase();
        // }

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
            Engine::Ressources::GameObject& addChild(Args&&... args) noexcept
            {
                (*this).children.emplace_back(std::make_unique<T>(args...));
                (*this).children.back()->children = std::list<std::unique_ptr<Engine::Ressources::GameObject>>();
                (*this).children.back()->update((*this).getModelMatrix());

                return *(*this).children.back();
            }

        bool 		operator==		(GameObject const& other)
        {
            return name_ == other.getName();
        }

        // TODO: Component[] getComponents()

        std::list<std::unique_ptr<Component>>& getComponents () noexcept {return components;}
        const std::list<std::unique_ptr<Component>>& getComponents () const noexcept {return components;}
        
    private:
        std::list<std::unique_ptr<Component>> components;
    };


} // namespace Engine::Ressources

#endif //_GAME_OBJECT_H