//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-18 - 08 h 04

#ifndef _RENDERING_SYSTEM_H
#define _RENDERING_SYSTEM_H

#include <vector>
#include <map>
#include "GE/Core/Component/RenderingObject.hpp"
#include "GE/LowRenderer/camera.hpp"
#include "GE/LowRenderer/model.hpp"

namespace Engine::Core::System
{
    class RenderingSystem
    {
        protected:

        #pragma region attribut
        #pragma endregion //!attribut

        #pragma region static attribut

        static std::vector<Engine::Core::Component::RenderingObject*> pRenderingObjects;

        #pragma endregion //! static attribut

        #pragma region methods
        #pragma endregion //!methods

        public:

        #pragma region constructor/destructor

        RenderingSystem ()					                        = delete;
        RenderingSystem (const RenderingSystem& other)			    = delete;
        RenderingSystem (RenderingSystem&& other)				    = delete;
        virtual ~RenderingSystem ()				                    = delete;
        RenderingSystem& operator=(RenderingSystem const& other)    = delete;
        RenderingSystem& operator=(RenderingSystem && other)		= delete;

        #pragma endregion //!constructor/destructor

        #pragma region methods

        static
        void draw () noexcept
        {
            std::map<float, Engine::Core::Component::RenderingObject*> mapElemSortedByDistance;

            glDisable(GL_BLEND);
            sortAndDrawOpqueElement(mapElemSortedByDistance);
            glEnable(GL_BLEND);

            for(std::map<float, Engine::Core::Component::RenderingObject*>::reverse_iterator it = mapElemSortedByDistance.rbegin(); it != mapElemSortedByDistance.rend(); ++it) 
            {
                it->second->draw();
            }
        }

        /**
         * @brief Draw entity if entity is drawable
         * 
         */
        static
        void sortAndDrawOpqueElement(std::map<float, Engine::Core::Component::RenderingObject*> &mapElemSortedByDistance)
        {
            for (Engine::Core::Component::RenderingObject* pRenderingObject : pRenderingObjects)
            {
                Engine::LowRenderer::Model *model = dynamic_cast<Engine::LowRenderer::Model*>(pRenderingObject);
                if (model != nullptr)
                {
                    if (model->isOpaque())
                        model->draw();
                    else
                    {
                        float distance = (Engine::LowRenderer::Camera::getCamUse()->getPosition() - (pRenderingObject->getGameObject().getPosition())).length();
                        mapElemSortedByDistance[distance] = model;
                    }
                }
            }
        }

        /**
         * @brief Add RenderingObject (using key word this) on the physic system. This object will be update by the physic system
         * 
         * @param pRenderingObject 
         */
        static
        void addRenderingObject (Engine::Core::Component::RenderingObject* pRenderingObject) noexcept
        {
            pRenderingObjects.push_back(pRenderingObject);
        }

        static
        void updateRenderingObjectPointor (Engine::Core::Component::RenderingObject* newPointorRenderingObject, Engine::Core::Component::RenderingObject* exPointorRenderingObject) noexcept
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

        static
        void removeRenderingObject (Engine::Core::Component::RenderingObject* pRenderingObject) noexcept
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

        #pragma endregion //!methods

        #pragma region accessor
        #pragma endregion //!accessor

        #pragma region mutator
        #pragma endregion //!mutator

        #pragma region operator
        #pragma endregion //!operator

        #pragma region convertor
        #pragma endregion //!convertor


    };

} /*namespace Engine::Core::System*/

#endif //_RENDERING_SYSTEM_H