#include "GE/Core/Component/RenderingObject.hpp"
#include "GE/Core/System/RenderingSystem.hpp"

using namespace Engine::Core::Component;
using namespace Engine::Core::System;
using namespace Engine::Ressources;

RenderingObject::RenderingObject(GameObject &refGameObject)
    : Component  (refGameObject)
{
    RenderingSystem::addRenderingObject(this);
}

RenderingObject::RenderingObject(const RenderingObject &other)
    : Component  (other._gameObject)
{
    RenderingSystem::addRenderingObject(this);
}

RenderingObject::RenderingObject(RenderingObject &&other)
    : Component  (other._gameObject)
{
    RenderingSystem::addRenderingObject(this);
}

RenderingObject::~RenderingObject()
{
    RenderingSystem::removeRenderingObject(this);
}