//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-30 - 10 h 09

#ifndef _COLLISION_H
#define _COLLISION_H

#include <algorithm>

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/Shape3D.hpp"
#include "GE/Core/Maths/LinkedShape3D.hpp"
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

        static bool IsSphereOrientedBoxCollided(Sphere sphere, OrientedBox box, CollisionPoints& _intersection);

        static bool IsSphereOrientedBoxCollided(LinkedSphere sphere, LinkedOrientedBox box, CollisionPoints& _intersection);


        #pragma endregion //!static methods

        protected:

        private:

    };

} /*namespace Engine::Core::Maths*/

#endif //_COLLISION_H