#include "GE/Core/Maths/ShapeRelation/AabbAabb.hpp"
#include "GE/Core/Maths/vec.hpp"

#include <algorithm>

using namespace Engine::Core::Maths;
using namespace Engine::Core::Maths::Shape3D;
using namespace Engine::Core::Maths::ShapeRelation;


bool AabbAabb::isBothAABBCollided(const AABB& aabb1, const AABB& aabb2, Intersection& intersection)
{
    /*Step 1 : Find if the both shape are non collided*/
    //looking for a collision between the bow and the AABB of the sphere.
    Vec3 BOmega = aabb2.getCenter() - aabb1.getCenter();

    if (std::abs(BOmega.x) > aabb1.getExtI() + aabb2.getExtI()  ||
        std::abs(BOmega.y) > aabb1.getExtJ() + aabb2.getExtJ()  ||
        std::abs(BOmega.z) > aabb1.getExtK() + aabb2.getExtK())
    {
        intersection.setNotIntersection();
        return false;
    }

    /*Step 2 : find the near point from box to the sphere*/
    Vec3 pointInBoxNearestOfAABB {  std::clamp(BOmega.x, -aabb1.getExtI(), aabb1.getExtI()),
                                    std::clamp(BOmega.y, -aabb1.getExtJ(), aabb1.getExtJ()),
                                    std::clamp(BOmega.z, -aabb1.getExtK(), aabb1.getExtK())};
    intersection.setOneIntersection(pointInBoxNearestOfAABB);
    return true;
}

bool AabbAabb::isBothAABBCollided(const Shape3D::AABB& aabb1, const Shape3D::AABB& aabb2)
{
    /*Step 1 : Find if the both shape are non collided*/
    //looking for a collision between the bow and the AABB of the sphere.
    Vec3 BOmega = aabb2.getCenter() - aabb1.getCenter();

    if (std::abs(BOmega.x) > aabb1.getExtI() + aabb2.getExtI() ||
        std::abs(BOmega.y) > aabb1.getExtJ() + aabb2.getExtJ() ||
        std::abs(BOmega.z) > aabb1.getExtK() + aabb2.getExtK())
    {
        return false;
    }
    return true;
}