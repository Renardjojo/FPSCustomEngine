//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-29 - 19 h 27

#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include <list>
#include <map>
#include <string>
#include <istream>
#include <sstream>
#include <iostream>
#include <memory>
#include "GE/LowRenderer/entity.hpp"
#include "GE/Core/Debug/log.hpp"
#include "GE/LowRenderer/camera.hpp"
#include "GE/LowRenderer/model.hpp"
#include "GE/Ressources/Component.hpp"

namespace Engine::Ressources
{

class GameObject
{
public:

    GameObject() = default;
    GameObject(const GameObject &other) = default;
    GameObject(GameObject &&other) = default;
    virtual ~GameObject() = default;
    GameObject &operator=(GameObject const &other) = default;
    GameObject &operator=(GameObject &&other) = default;
    std::unique_ptr<Engine::LowRenderer::Entity> entity;

    std::list<GameObject> children;
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
    void sortAndDrawOpqueElement(std::map<float, Engine::LowRenderer::Model *> &mapElemSortedByDistance) const
    {
        for (auto i = children.begin(); i != children.end(); i++)
        {
            Engine::LowRenderer::Model *model = dynamic_cast<Engine::LowRenderer::Model *>(i->entity.get());
            if (model != nullptr)
            {
                if (model->isOpaque())
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
    /**
     * @brief add a Component to the gameobject
     * 
     * @tparam T 
     * @tparam Args 
     * @param args 
     */
    template <typename T, typename... Args>
    void addComponent(Args &&... args) noexcept
    {
        components.push_back(std::make_unique<T>(*this, args...));
    }

    /**
     * @brief Get the first Component type object
     * 
     * @tparam T 
     * @return Component& 
     */
    template <typename T>
    Component* getComponent()
    {
        for (std::unique_ptr<Component> &uniquePtrComponent : components)
        {
            Component *comp = dynamic_cast<T>(uniquePtrComponent.get());

            if (comp) 
                return uniquePtrComponent.get();
        }
        return nullptr;
    }

    // TODO: Component[] getComponents()
private:
    std::vector<std::unique_ptr<Component>> components;
};

} // namespace Engine::Ressources

#endif //_GAMEOBJECT_H