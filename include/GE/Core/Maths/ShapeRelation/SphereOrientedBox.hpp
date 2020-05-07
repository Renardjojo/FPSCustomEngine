//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 16 h 05

#ifndef _SPHERE_ORIENTED_BOX_H
#define _SPHERE_ORIENTED_BOX_H

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/ShapeRelation/Intersection.hpp"
#include "GE/Core/Maths/Shape3D/Sphere.hpp"
#include "GE/Core/Maths/Shape3D/OrientedBox.hpp"

#include <algorithm>

namespace Engine::Core::Maths::ShapeRelation
{
    class SphereOrientedBox
    {
        public:

        #pragma region constructor/destructor

        SphereOrientedBox ()					                        = delete;
        SphereOrientedBox (const SphereOrientedBox& other)			    = delete;
        SphereOrientedBox (SphereOrientedBox&& other)				    = delete;
        virtual ~SphereOrientedBox ()				                    = delete;
        SphereOrientedBox& operator=(SphereOrientedBox const& other)    = delete;
        SphereOrientedBox& operator=(SphereOrientedBox && other)		= delete;

        #pragma endregion //!constructor/destructor

        #pragma region static methods

        static bool IsSphereOrientedBoxCollided(const Shape3D::Sphere& sphere, const Shape3D::OrientedBox& box, Intersection& intersection)
        {
            /*Step 1 : Find if the both shape are non collided*/
            //looking for a collision between the bow and the AABB of the sphere.
            Vec3 BOmega = Referential::GlobalToLocalPosition(box.getReferential(), sphere.getCenter());

            if (std::abs(BOmega.x) > box.getExtI() + sphere.getRadius() ||
                std::abs(BOmega.y) > box.getExtJ() + sphere.getRadius() ||
                std::abs(BOmega.z) > box.getExtK() + sphere.getRadius())
            {
                intersection.SetNotIntersection();
                return false;
            }

            /*Step 2 : find the near point from box to the sphere*/
            Vec3 pointInBoxNearestOfSphere = Vec3({ std::clamp(BOmega.x, -box.getExtI(), box.getExtI()),
                                                    std::clamp(BOmega.y, -box.getExtJ(), box.getExtJ()),
                                                    std::clamp(BOmega.z, -box.getExtK(), box.getExtK())});

            /*Step 3 : looking for a collision point*/
            if (std::abs(pointInBoxNearestOfSphere.length() - BOmega.length()) <= sphere.getRadius())
            {
                intersection.SetOneIntersection(Referential::LocalToGlobalPosition(box.getReferential(), pointInBoxNearestOfSphere));
                return true;
            }

            intersection.SetNotIntersection();
            return false;
        }

        #pragma endregion //!static methods

        private :

        #pragma region static methods
        #pragma endregion //!static methods
    };

} /*namespace Engine::Core::Maths::ShapeRelation*/

#endif //_SPHERE_ORIENTED_BOX_H