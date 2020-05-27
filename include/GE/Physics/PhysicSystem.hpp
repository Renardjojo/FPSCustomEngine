//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-29 - 10 h 53

#ifndef _PHYSICS_H
#define _PHYSICS_H

#include <vector>
#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/ShapeRelation/Intersection.hpp"
#include "GE/Physics/PhysicalObject.hpp"
#include "GE/Physics/ColliderShape/Collider.hpp"
#include "GE/Core/Maths/Shape3D/Segment.hpp"
#include "GE/Ressources/GameObject.hpp"

namespace Engine::Physics
{
    struct CollisionHitInfos
    {
        Engine::Core::Maths::ShapeRelation::Intersection    intersectionsInfo;
        PhysicalObject*                                     optionnalPhysicalObjectPtr1;
        PhysicalObject*                                     optionnalPhysicalObjectPtr2;
    };

    class PhysicSystem
    {
        public:

        #pragma region constructor/destructor

        PhysicSystem ()					                = delete;
        PhysicSystem (const PhysicSystem& other)	    = delete;
        PhysicSystem (PhysicSystem&& other)			    = delete;
        virtual ~PhysicSystem ()				        = delete;
        PhysicSystem& operator=(PhysicSystem const&)	= delete;
        PhysicSystem& operator=(PhysicSystem &&)		= delete;

        #pragma endregion //!constructor/destructor

        #pragma region methods

        static
        void update () noexcept;

        #pragma endregion //!methods

        #pragma region accessor
        #pragma endregion //!accessor

        #pragma region mutator

        /**
         * @brief Add physicalObject (using key word this) on the physic system. This object will be update by the physic system
         * 
         * @param pPhysicalObject 
         */
        static
        void addPhysicalObject (PhysicalObject* pPhysicalObject) noexcept
        {
            pPhysicalObjects.push_back(pPhysicalObject);
        }

        static
        void updatePhysicalObjectPointor (PhysicalObject* newPointorPhysicalObject, PhysicalObject* exPointorPhysicalObject) noexcept
        {
            for (std::vector<PhysicalObject*>::iterator it = pPhysicalObjects.begin(); it != pPhysicalObjects.end(); it++)
            {
                if ((*it) == exPointorPhysicalObject)
                {
                    *it = newPointorPhysicalObject;
                    return;
                }
            }
        }

        static
        void removePhysicalObject (PhysicalObject* pPhysicalObject) noexcept
        {
            for (std::vector<PhysicalObject*>::iterator it = pPhysicalObjects.begin(); it != pPhysicalObjects.end(); it++)
            {
                if ((*it) == pPhysicalObject)
                {
                    pPhysicalObjects.erase(it);
                    return;
                }
            }
        }

        static
        void addCollider (ColliderShape::Collider* pCollider) noexcept
        {
            pColliders.push_back(pCollider);
        }

        static
        void updateColliderPointor (ColliderShape::Collider* newPointorCollider, ColliderShape::Collider* exPointorCollider) noexcept
        {
            for (std::vector<ColliderShape::Collider*>::iterator it = pColliders.begin(); it != pColliders.end(); it++)
            {
                if ((*it) == exPointorCollider)
                {
                    *it = newPointorCollider;
                    return;
                }
            }
        }

        static
        void removeCollider (ColliderShape::Collider* pCollider) noexcept
        {
            for (std::vector<ColliderShape::Collider*>::iterator it = pColliders.begin(); it != pColliders.end(); it++)
            {
                if ((*it) == pCollider)
                {
                    pColliders.erase(it);
                    return;
                }
            }
        }

        static
        bool rayCast(const Engine::Core::Maths::Shape3D::Segment& ray, Engine::Physics::ColliderShape::HitInfo& rayHitInfo) noexcept;

        static
        bool rayCast(const Engine::Core::Maths::Vec3& origin, const Engine::Core::Maths::Vec3& direction, float maxDistance, Engine::Physics::ColliderShape::HitInfo& rayHitInfo) noexcept;

        static
        bool rayCast(const Engine::Core::Maths::Vec3& pt1, const Engine::Core::Maths::Vec3& pt2, Engine::Physics::ColliderShape::HitInfo& rayHitInfo) noexcept;

        /*Trigger ray cast call onCollisionEnter*/
        static
        bool triggerRayCast(Engine::Ressources::GameObject* pTriggerGameObject, const Engine::Core::Maths::Shape3D::Segment& ray, Engine::Physics::ColliderShape::HitInfo& rayHitInfo) noexcept;

        static
        bool triggerRayCast(Engine::Ressources::GameObject* pTriggerGameObject, const Engine::Core::Maths::Vec3& origin, const Engine::Core::Maths::Vec3& direction, float maxDistance, Engine::Physics::ColliderShape::HitInfo& rayHitInfo) noexcept;

        static
        bool triggerRayCast(Engine::Ressources::GameObject* pTriggerGameObject, const Engine::Core::Maths::Vec3& pt1, const Engine::Core::Maths::Vec3& pt2, Engine::Physics::ColliderShape::HitInfo& rayHitInfo) noexcept;

        /*Trigger ray cast call onCollisionEnter with only tag*/
        static
        bool triggerRayCast(const std::string& tag, const Engine::Core::Maths::Shape3D::Segment& ray, Engine::Physics::ColliderShape::HitInfo& rayHitInfo) noexcept;

        static
        bool triggerRayCast(const std::string& tag, const Engine::Core::Maths::Vec3& origin, const Engine::Core::Maths::Vec3& direction, float maxDistance, Engine::Physics::ColliderShape::HitInfo& rayHitInfo) noexcept;

        static
        bool triggerRayCast(const std::string& tag, const Engine::Core::Maths::Vec3& pt1, const Engine::Core::Maths::Vec3& pt2, Engine::Physics::ColliderShape::HitInfo& rayHitInfo) noexcept;


        /*TODO: Can be implemented if all segment/Shape have the good collision function implemented without intersection
        static
        bool rayCast(const Engine::Core::Maths::Shape3D::Segment& ray) noexcept;

        static
        bool rayCast(const Engine::Core::Maths::Vec3& origin, const Engine::Core::Maths::Vec3& direction, float maxDistance) noexcept;

        static
        bool rayCast(const Engine::Core::Maths::Vec3& pt1, const Engine::Core::Maths::Vec3& pt2) noexcept;
        */

        static 
        const Engine::Core::Maths::Vec3& getGravity() noexcept { return gravity;}

        static
        void setGravity(const Engine::Core::Maths::Vec3& newGravity) noexcept { gravity = newGravity;}

        #pragma endregion //!mutator

        #pragma region operator
        #pragma endregion //!operator

        #pragma region convertor
        #pragma endregion //!convertor

        protected:

        #pragma region attribut

        static Engine::Core::Maths::Vec3 gravity;

        static std::vector<PhysicalObject*> pPhysicalObjects;
        static std::vector<ColliderShape::Collider*> pColliders;

        static float test;

        #pragma endregion //!attribut

        #pragma region static attribut
        #pragma endregion //! static attribut

        #pragma region methods
        #pragma endregion //!methods

        private:

    };
}

#endif //_PHYSICS_H