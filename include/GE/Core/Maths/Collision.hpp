//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-30 - 10 h 09

#ifndef _COLLISION_H
#define _COLLISION_H

#include <algorithm>

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/Shape3D.hpp"
#include "GE/Core/Maths/Referential.hpp"
#include "GE/Ressources/GameObject.hpp"

namespace Engine::Core::Maths
{
    enum class ECollisionType
    {
        NoIntersection,
        OneIntersectiont,
        TwoIntersectiont,
        InfinyIntersection,
        UnknowIntersection
    };

    struct CollisionPoints 
    {
        ECollisionType intersectionType = ECollisionType::NoIntersection;
        Vec3 intersection1;
        Vec3 intersection2;

        void SetNotIntersection ()
        {
            intersectionType = ECollisionType::NoIntersection;
        }

        void SetOneIntersection(Vec3 intersectionPoint)
        {
            intersectionType = ECollisionType::OneIntersectiont;
            intersection1 = intersectionPoint;
        }

        void SetTwoIntersection(Vec3 intersectionPoint1, Vec3 intersectionPoint2)
        {
            intersectionType = ECollisionType::TwoIntersectiont;
            intersection1 = intersectionPoint1;
            intersection2 = intersectionPoint2;
        }

        void SetSecondIntersection(Vec3 intersectionPoint2)
        {
            intersectionType = ECollisionType::TwoIntersectiont;
            intersection2 = intersectionPoint2;
        }

        void SetInifitIntersection()
        {
            intersectionType = ECollisionType::InfinyIntersection;
        }

        void SetUnKnowIntersection()
        {
            intersectionType = ECollisionType::UnknowIntersection;
        }
    };

    struct HitInfo
    {
        CollisionPoints                 collisionPoints;
        Engine::Ressources::GameObject& gameObject1;
        Engine::Ressources::GameObject& gameObject2;
    };

    class Collision
    {
        public:

        #pragma region constructor/destructor

        Collision ()					                = delete;
        Collision (const Collision& other)			    = delete;
        Collision (Collision&& other)				    = delete;
        virtual ~Collision ()				            = delete;
        Collision& operator=(Collision const& other)	= delete;
        Collision& operator=(Collision && other)		= delete;

        #pragma endregion //!constructor/destructor

        #pragma region static methods

        static bool IsSphereOrientedBoxCollided(Sphere sphere, OrientedBox box, CollisionPoints& _intersection)
        {
            /*Step 1 : Find if the both shape are non collided*/
            //looking for a collision between the bow and the AABB of the sphere.
            Vec3 BOmega = Referential::GlobalToLocalPosition(box.getReferential(), sphere.getCenter());

            if (std::abs(BOmega.x) > box.getExtensionValueI() + sphere.getRadius() ||
                std::abs(BOmega.y) > box.getExtensionValueJ() + sphere.getRadius() ||
                std::abs(BOmega.z) > box.getExtensionValueK() + sphere.getRadius())
            {
                _intersection.SetNotIntersection();
                return false;
            }

            /*Step 2 : find the near point from box to the sphere*/
            Vec3 pointInBoxNearestOfSphere = Vec3({ std::clamp(BOmega.x, -box.getExtensionValueI(), box.getExtensionValueI()),
                                                    std::clamp(BOmega.y, -box.getExtensionValueJ(), box.getExtensionValueJ()),
                                                    std::clamp(BOmega.z, -box.getExtensionValueK(), box.getExtensionValueK())});

            /*Step 3 : looking for a collision point*/
            if (std::abs(pointInBoxNearestOfSphere.length() - BOmega.length()) <= sphere.getRadius())
            {
                _intersection.SetOneIntersection(Referential::LocalToGlobalPosition(box.getReferential(), pointInBoxNearestOfSphere));
                return true;
            }

            _intersection.SetNotIntersection();
            return false;
        }



        #pragma endregion //!static methods

        protected:

        private:

    };

} /*namespace Engine::Core::Maths*/

#endif //_COLLISION_H