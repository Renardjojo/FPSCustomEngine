#include "GE/Core/Maths/Shape3D/Plane.hpp"
#include "GE/Core/Maths/mat.hpp"
#include <limits>

using namespace Engine::Core::Maths;
using namespace Engine::Core::Maths::Shape3D;

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