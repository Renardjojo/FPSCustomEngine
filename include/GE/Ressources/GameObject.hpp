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

        /**
         * @brief update entity and these child if current entity is dirty
         * 
         */
        virtual void updateSelfAndChild() noexcept
        {
            for (std::list<std::unique_ptr<GameObject>>::iterator i = children.begin(); i != children.end(); i++)
            {
                if ((*i)->isDirty())
                {
                    if((*i)->_isDead)
                    {
                        i = children.erase(i);
                        continue;
                    }

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
            _components.emplace_back(std::make_unique<T>(*this, args...));
            return *dynamic_cast<T*>(_components.back().get());
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
            for (std::unique_ptr<Component> &uniquePtrComponent : _components)
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
        void destroyChild (const std::string& path) noexcept
        {
            GE_assert(!path.empty());

            std::stringstream sPath(path);
            std::string word;
            Engine::Ressources::GameObject* parentEntity = this;
            Engine::Ressources::GameObject* currentEntity = this;
            std::list<std::unique_ptr<GameObject>>::iterator it;

            while (std::getline(sPath, word, '/'))
            {
                if (word.empty() || word == "." || word == name_) continue;

                bool isFound = false;
                parentEntity = currentEntity;

                for (it = parentEntity->children.begin(); it != parentEntity->children.end(); it++)
                {
                    if ((*it)->getName() == word)
                    {
                        currentEntity = it->get();
                        isFound = true;
                        break;
                    }
                }
                    
                if (!isFound)
                {
                    Engine::Core::Debug::SLog::logWarning(std::string("Canno't found \"") + word + "\" in gameObject \"" + name_ + "\"" + " with path : \"" + path + "\"" );
                    return;
                }
            }

            parentEntity->children.erase(it);
        }

        /**
         * @brief destroy childen of gameobject
         * 
         * @param GameObject 
         */
        std::list<std::unique_ptr<GameObject>>::iterator destroyChild (GameObject* pGameObject) noexcept
        {
            for (std::list<std::unique_ptr<GameObject>>::iterator it =  children.begin(); it != children.end(); it++)
            {
                if ((*it).get() == pGameObject)
                {
                    return children.erase(it);
                }
            }
            return children.end();
        }

        std::list<std::unique_ptr<GameObject>>::iterator destroyChild (const std::list<std::unique_ptr<GameObject>>::iterator& it) noexcept
        {
            return children.erase(it);
        }

        /**
         * @brief destroy component of gameobject
         * 
         * @param Component 
         */
        std::list<std::unique_ptr<Component>>::iterator destroyComponent (Component* pComponent) noexcept
        {
            for (std::list<std::unique_ptr<Component>>::iterator it =  _components.begin(); it != _components.end(); it++)
            {
                if ((*it).get() == pComponent)
                {
                    return _components.erase(it);
                }
            }
            return _components.end();
        }

        std::list<std::unique_ptr<Component>>::iterator destroyComponent (const std::list<std::unique_ptr<Component>>::iterator& it) noexcept
        {
            return _components.erase(it);
        }

        virtual 
        void destroy() noexcept
        {
            /*set flag to be delete by it parent*/
            _isDead = true;
            isDirty_ = true;
        }

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
            (*this).children.back()->parent = this;
            return *(*this).children.back();
        }

        bool 		operator==		(GameObject const& other)
        {
            return (this == &other);
        }

        template <typename T>
        std::vector<T*> getComponents()
        {
            std::vector<T*> toReturn;
            for (std::unique_ptr<Component> &uniquePtrComponent : _components)
            {
                T* comp = dynamic_cast<T*>(uniquePtrComponent.get());

                if (comp != nullptr)
                {
                    toReturn.push_back(comp);
                }
            }
            return toReturn;
        }

        std::list<std::unique_ptr<Component>>& getComponents () noexcept {return _components;}
        const std::list<std::unique_ptr<Component>>& getComponents () const noexcept {return _components;}

        std::string getRelativePath()
        {
            std::string path = this->getName();
            GameObject* parentIt = this->parent;

            while (parentIt)
            {
                path = parentIt->getName() + std::string("/") + path;
                parentIt = parentIt->parent;
            }

            return path;
        }
        
        void setTag(const std::string& newTag) { _tag = newTag; }
        std::string& getTag() { return _tag; } 

        bool CompareTag(const std::string& toCompare)
        {
            if (toCompare.compare(_tag) == 0)
                return true;
            return false;
        }

        GameObject* parent;
        std::list<std::unique_ptr<GameObject>> children;

    protected:

        std::list<std::unique_ptr<Component>>   _components;
        std::string                             _tag;
        bool                                    _isDead {false}; //Flag that inform it parent that this transform must be destroy on update loop
    };
} // namespace Engine::Ressources

#endif //_GAME_OBJECT_H