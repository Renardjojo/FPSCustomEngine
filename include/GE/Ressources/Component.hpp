//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-29 - 18 h 54

#ifndef _COMPONENT_H
#define _COMPONENT_H

namespace Engine::Ressources
{
class GameObject;

class Component
{
public:

    Component(GameObject &refGameObject)
    : gameObject {refGameObject} 
    {}

    Component(const Component &other)               = default;
    Component(Component &&other)                    = default;
    virtual ~Component()                            = default;

    Component()                                     = delete;
    Component &operator=(Component const &other)    = delete;
    Component &operator=(Component &&other)         = delete;

    GameObject &        getGameObject() noexcept { return gameObject; }
    const GameObject&   getGameObject() const noexcept { return gameObject; }

protected:

    GameObject& gameObject;

};

} //

#endif //_COMPONENT_H