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
        void draw () noexcept;

        /**
         * @brief Draw entity if entity is drawable
         * 
         */
        static
        void sortAndDrawOpqueElement(std::map<float, Engine::Core::Component::RenderingObject*> &mapElemSortedByDistance);

        /**
         * @brief Add RenderingObject (using key word this) on the physic system. This object will be update by the physic system
         * 
         * @param pRenderingObject 
         */
        static
        void addRenderingObject (Engine::Core::Component::RenderingObject* pRenderingObject) noexcept;

        static
        void updateRenderingObjectPointor (Engine::Core::Component::RenderingObject* newPointorRenderingObject, Engine::Core::Component::RenderingObject* exPointorRenderingObject) noexcept;

        static
        void removeRenderingObject (Engine::Core::Component::RenderingObject* pRenderingObject) noexcept;

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