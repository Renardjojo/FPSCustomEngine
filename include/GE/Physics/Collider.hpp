//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-30 - 09 h 51

#ifndef _COLLIDER_H
#define _COLLIDER_H

#include "GE/Ressources/Component.hpp"
#include "GE/Physics/PhysicalObject.hpp"
#include "GE/Core/Maths/Shape3D/Linked/LinkedAABB.hpp"
#include "GE/Core/Maths/Shape3D/Linked/LinkedOrientedBox.hpp"
#include "GE/Core/Maths/Shape3D/Linked/LinkedSphere.hpp"

namespace Engine::Physics
{

class Collider : public Engine::Ressources::Component
{
public:

    Collider(Engine::Ressources::GameObject &refGameObject);
    Collider(const Collider &other);
    Collider(Collider &&other);
    virtual ~Collider();
    
    Collider() = delete;

    virtual bool isCollidingWith(Collider& collider) { return false; }
    //virtual bool isCollidingWith(OrientedBoxCollider& collider) { return false; }

    PhysicalObject* GetAttachedPhysicalObject() { return attachedPhysicalObject; }

    float GetBounciness() { return bounciness_; }
    void SetBounciness(float bounciness) { bounciness_ = bounciness; }

protected:

    PhysicalObject* attachedPhysicalObject;

    float bounciness_ = 0.f;
    float friction_;

};

class SphereCollider : public Collider
{

public:

    SphereCollider(Engine::Ressources::GameObject &refGameObject);
    SphereCollider(const SphereCollider &other);
    SphereCollider(SphereCollider &&other);

    bool isCollidingWith(Collider& collider) final;
    //bool isCollidingWith(OrientedBoxCollider& collider) final;

    virtual ~SphereCollider();
    
    Engine::Core::Maths::Shape3D::Linked::LinkedSphere& GetSphere() { return sphere_; }

    Engine::Core::Maths::Shape3D::Linked::LinkedSphere sphere_;
private:


};

class OrientedBoxCollider : public Collider
{

public:

    OrientedBoxCollider(Engine::Ressources::GameObject &refGameObject);
    OrientedBoxCollider(const OrientedBoxCollider &other);
    OrientedBoxCollider(OrientedBoxCollider &&other);
    virtual ~OrientedBoxCollider();
    
    OrientedBoxCollider() = delete;

    bool isCollidingWith(Collider& collider) final;
    // bool isCollidingWith(OrientedBoxCollider& collider) final

    Engine::Core::Maths::Shape3D::Linked::LinkedOrientedBox& GetBox() { return orientedbox_; }

    Engine::Core::Maths::Shape3D::Linked::LinkedOrientedBox orientedbox_;
private:


};

} /*namespace */

#endif //_COLLIDER_H