#include "GE/Core/System/RenderingSystem.hpp"

using namespace Engine::Core::System;

void RenderingSystem::draw () noexcept
{
    std::map<float, Engine::Core::Component::RenderingObject*> mapElemSortedByDistance;

    glDisable(GL_BLEND);
    sortAndDrawOpqueElement(mapElemSortedByDistance);
    glEnable(GL_BLEND);

    for(std::map<float, Engine::Core::Component::RenderingObject*>::reverse_iterator it = mapElemSortedByDistance.rbegin(); it != mapElemSortedByDistance.rend(); ++it) 
    {
        if (it->second->isActivate())
            it->second->draw();
    }
}

void RenderingSystem::sortAndDrawOpqueElement(std::map<float, Engine::Core::Component::RenderingObject*> &mapElemSortedByDistance)
{
    for (Engine::Core::Component::RenderingObject* pRenderingObject : pRenderingObjects)
    {
        Engine::LowRenderer::Model *model = dynamic_cast<Engine::LowRenderer::Model*>(pRenderingObject);
        if (model != nullptr)
        {
            if (model->isOpaque() && model->isActivate())
                model->draw();
            else
            {
                float distance = (Engine::LowRenderer::Camera::getCamUse()->getPosition() - (pRenderingObject->getGameObject().getPosition())).length();
                mapElemSortedByDistance[distance] = model;
            }
        }
    }
}

void RenderingSystem::addRenderingObject (Engine::Core::Component::RenderingObject* pRenderingObject) noexcept
{
    pRenderingObjects.push_back(pRenderingObject);
}

void RenderingSystem::updateRenderingObjectPointor (Engine::Core::Component::RenderingObject* newPointorRenderingObject, Engine::Core::Component::RenderingObject* exPointorRenderingObject) noexcept
{
    for (std::vector<Engine::Core::Component::RenderingObject*>::iterator it = pRenderingObjects.begin(); it != pRenderingObjects.end(); it++)
    {
        if ((*it) == exPointorRenderingObject)
        {
            *it = newPointorRenderingObject;
            return;
        }
    }
}

void RenderingSystem::removeRenderingObject (Engine::Core::Component::RenderingObject* pRenderingObject) noexcept
{
    for (std::vector<Engine::Core::Component::RenderingObject*>::iterator it = pRenderingObjects.begin(); it != pRenderingObjects.end(); it++)
    {
        if ((*it) == pRenderingObject)
        {
            pRenderingObjects.erase(it);
            return;
        }
    }
}

std::vector<Engine::Core::Component::RenderingObject*> RenderingSystem::pRenderingObjects {};
