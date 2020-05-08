#include "GE/Core/Maths/ShapeRelation/SphereOrientedBox.hpp"

#include "GE/Core/Maths/vec.hpp"
#include <algorithm>

using namespace Engine::Core::Maths;
using namespace Engine::Core::Maths::Shape3D;
using namespace Engine::Core::Maths::ShapeRelation;

bool SphereOrientedBox::isSphereOrientedBoxCollided(const Sphere& sphere, const OrientedBox& box, Intersection& intersection)
{
    /*Step 1 : Find if the both shape are non collided*/
    //looking for a collision between the bow and the AABB of the sphere.
    Vec3 BOmega = Referential::globalToLocalPosition(box.getReferential(), sphere.getCenter());

    if (std::abs(BOmega.x) > box.getExtI() + sphere.getRadius() ||
        std::abs(BOmega.y) > box.getExtJ() + sphere.getRadius() ||
        std::abs(BOmega.z) > box.getExtK() + sphere.getRadius())
    {
        intersection.setNotIntersection();
        return false;
    }

    /*Step 2 : find the near point from box to the sphere*/
    Vec3 pointInBoxNearestOfSphere = Vec3({ std::clamp(BOmega.x, -box.getExtI(), box.getExtI()),
                                            std::clamp(BOmega.y, -box.getExtJ(), box.getExtJ()),
                                            std::clamp(BOmega.z, -box.getExtK(), box.getExtK())});

    /*Step 3 : looking for a collision point*/
    if (std::abs(pointInBoxNearestOfSphere.length() - BOmega.length()) <= sphere.getRadius())
    {
        intersection.setOneIntersection(Referential::localToGlobalPosition(box.getReferential(), pointInBoxNearestOfSphere));
        return true;
    }

    intersection.setNotIntersection();
    return false;
}