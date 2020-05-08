#include "GE/Core/Maths/ShapeRelation/SegmentSegment.hpp"

using namespace Engine::Core::Maths;
using namespace Engine::Core::Maths::ShapeRelation;

static float d(const Vec3& m, const Vec3& n, const Vec3& o, const Vec3& p)
{
    return (m.x - n.x) * (o.x - p.x) + (m.y - n.y) * (o.y - p.y) + (m.z - n.z) * (o.z - p.z);
}

bool SegmentSegment::isSegmentSegmentCollided(const Vec3& a1, const Vec3& a2, const Vec3& b1, const Vec3& b2, Intersection& intersection)
{
    /*using distance between line algorythme. Found reference of this algorythme on : http://paulbourke.net/geometry/pointlineplane/*/

    float sub1 = (d(a2, a1, a2, a1) * d(b2, b1, b2, b1) - d(b2, b1, a2, a1) * d(b2, b1, a2, a1));

    /* if b dot d == 0, it means the lines are parallel so have infinite intersection points*/
    if (sub1 == 0)
        return false;

    float t1 = (d(a1, b1, b2, b1) * d(b2, b1, a2, a1) - d(a1, b1, a2, a1) * d(b2, b1, b2, b1)) / sub1;

    float sub2 = d(b2, b1, b2, b1);

    /* if b dot d == 0, it means the lines are parallel so have infinite intersection points*/
    if (sub2 == 0)
        return false;

    float t2 = (d(a1, b1, b2, b1) + t1 * d(b2, b1, a2, a1)) / sub2;

    if (t1 < 0 || t1 > 1 || t2 < 0 || t2 > 1)
        return false;

    /*OM = OA + t(OB - OA)*/
    if (intersection.intersectionType == EIntersectionType::OneIntersectiont)
    {
        intersection.setSecondIntersection(a1 + t1 * (a2 - a1));
    }
    else
    {
        intersection.setOneIntersection(a1 + t1 * (a2 - a1));
    }

    return true;
}
