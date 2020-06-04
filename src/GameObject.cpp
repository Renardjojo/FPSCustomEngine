#include "GE/Ressources/GameObject.hpp"

#include <istream>
#include <sstream>
#include <iostream>
#include "GE/Core/Debug/log.hpp"

using namespace Engine::Ressources;

void GameObject::updateSelfAndChild() noexcept
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

void GameObject::forceUpdate()
{
    for (auto&& i = children.begin(); i != children.end(); i++)
    {
        (*i)->update(modelMat_);
        (*i)->forceUpdate();
    }
}

GameObject* GameObject::getChild (const std::string& path) noexcept
{
    GE_assert(!path.empty());

    std::stringstream sPath(path);
    std::string word;
    GameObject* currentEntity = this;

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

void GameObject::destroyChild (const std::string& path) noexcept
{
    GE_assert(!path.empty());

    std::stringstream sPath(path);
    std::string word;
    GameObject* parentEntity = this;
    GameObject* currentEntity = this;
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

std::list<std::unique_ptr<GameObject>>::iterator GameObject::destroyChild (GameObject* pGameObject) noexcept
{
    for (std::list<std::unique_ptr<GameObject>>::iterator it = children.begin(); it != children.end(); it++)
    {
        if ((*it).get() == pGameObject)
        {
            return children.erase(it);
        }
    }
    return children.end();
}

std::list<std::unique_ptr<GameObject>>::iterator GameObject::destroyChild (const std::list<std::unique_ptr<GameObject>>::iterator& it) noexcept
{
    return children.erase(it);
}

std::list<std::unique_ptr<Component>>::iterator GameObject::destroyComponent (Component* pComponent) noexcept
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

void GameObject::setActive(bool newState)
{
    for (auto &&i : _components)
    {
        i->setActive(newState);
    }
}

std::list<std::unique_ptr<Component>>::iterator GameObject::destroyComponent (const std::list<std::unique_ptr<Component>>::iterator& it) noexcept
{
    return _components.erase(it);
}

void GameObject::destroy() noexcept
{
    /*set flag to be delete by it parent*/
    _isDead = true;
    isDirty_ = true;
}

void GameObject::destroyImmediate() noexcept
{
    parent->destroyChild(this);
}

bool 		GameObject::operator==		(GameObject const& other)
{
    return (this == &other);
}

std::string GameObject::getRelativePath()
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

bool GameObject::compareTag(const std::string& toCompare)
{
    if (toCompare.compare(_tag) == 0)
        return true;
    return false;
}