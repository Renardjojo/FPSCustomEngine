//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-29 - 18 h 54

#ifndef _COMPONENT_H
#define _COMPONENT_H

#include "save/rapidxml-1.13/rapidxml.hpp"
#include "save/rapidxml-1.13/rapidxml_print.hpp"
#include "save/rapidxml-1.13/rapidxml_utils.hpp"

using namespace rapidxml;

namespace Engine::Ressources
{
    class GameObject;

    class Component
    {
    public:

    Component(GameObject &refGameObject)
    : _gameObject {refGameObject} 
    {}

    Component(const Component &other)               = default;
    Component(Component &&other)                    = default;
    virtual ~Component()                            = default;

    Component()                                     = delete;
    Component &operator=(Component const &other)    = delete;
    Component &operator=(Component &&other)         = delete;

    GameObject &        getGameObject() noexcept { return _gameObject; }
    const GameObject&   getGameObject() const noexcept { return _gameObject; }

    protected:

    GameObject& _gameObject;

    };

} // namespace Engine::Ressources

#endif //_COMPONENT_H