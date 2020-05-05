#include "GE/Core/Maths/plane.hpp"
#include "GE/Core/Maths/mat.hpp"
#include <limits>

using namespace Engine::Core::Maths;

Plane::Plane (float distance, const Vec3& normal)
    :   normal_     (normal.getNormalize()),
        distance_   (distance)
{}

Plane::Plane (const Vec3& p1, const Vec3& p2, const Vec3& p3)
    :   normal_     ((p2 - p1).getCross(p3 - p1).getNormalize()),
        distance_   (p1.dotProduct(normal_))
{}


Plane::Plane (const Vec3& p1, const Vec3& normal)
    :   normal_     (normal.getNormalize()),
        distance_   (p1.dotProduct(normal_))
{}


void Plane::getPlansIntersection (const Plane& plane1, const Plane& plane2, const Plane& plane3, Vec3** point)
{
    Mat3 eqCoef {plane1.normal_, plane2.normal_, plane3.normal_};
    Vec3 eqRslt {plane1.distance_, plane2.distance_, plane3.distance_};
    if(eqCoef.inverse(eqCoef) == false)
    {
        *point = nullptr;
        return;
    }
    **point = eqCoef * eqRslt;
}

float Plane::getSignedDistanceToPlane(const Plane& plane, const Vec3& point)
{
    return plane.distance_ - point.dotProduct(plane.normal_); 
}

Vec3 Plane::getClosestPointToPlane(const Plane& plane, const Vec3& point)
{
    return point + getSignedDistanceToPlane(plane, point) * plane.normal_;
}

float Plane::getDistanceFromPointToPlan(const Plane& plane, const Vec3& point)
{
    return plane.normal_.dotProduct(point);
}

bool Plane::isOnPlane (const Plane& plane, const Vec3& point)
{
    float rst = plane.normal_.dotProduct(point);
    return rst < plane.distance_ + std::numeric_limits<float>::epsilon() && rst > plane.distance_ - std::numeric_limits<float>::epsilon();
}

bool Plane::vecIsBetweenPlanes (const Plane& plane1, const Plane& plane2, const Vec3& pt)
{
    return getSignedDistanceToPlane(plane1, pt) >= std::numeric_limits<float>::epsilon() && getSignedDistanceToPlane(plane2, pt) >= std::numeric_limits<float>::epsilon();
}
