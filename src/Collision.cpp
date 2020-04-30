#include "GE/Core/Maths/Collision.hpp"

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