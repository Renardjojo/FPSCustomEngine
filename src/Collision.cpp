#include "GE/Core/Maths/Collision.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/Shape3D.hpp"
#include "GE/Core/Maths/LinkedShape3D.hpp"


using namespace Engine::Core::Maths;

bool Collision::IsSphereOrientedBoxCollided(Sphere sphere, OrientedBox box, CollisionPoints& _intersection)
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

// linked
bool Collision::IsSphereOrientedBoxCollided(LinkedSphere sphere, LinkedOrientedBox box, CollisionPoints& _intersection)
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

bool Collision::GetFirstCollisionPointSphereBox(LinkedOrientedBox box, LinkedSphere sphere, Vec3 vectSphereSpeed, float deltaTime, HitInfo& hit)
{
    CollisionPoints collisionPoint;

    box.setLocalExtensionValueI(box.getLocalExtensionValueI() + sphere.getRadius());
    box.setLocalExtensionValueJ(box.getLocalExtensionValueJ() + sphere.getRadius());
    box.setLocalExtensionValueK(box.getLocalExtensionValueK() + sphere.getRadius());

    // if (!Collision::IsSegmentIntersectingOrientedBox(sphere, box, collisionPoint))
    //     return false;

    // detect if intersection point is on voroin face
    // is recherched point

    // if not detect if intersecting 1 or 3 near captulse

    // return info
    //collisionPoint.
    return false;
}
