#include "GE/Core/Maths/ShapeRelation/SegmentPlane.hpp"

#include "GE/Core/Maths/vec.hpp"
#include <limits>

using namespace Engine::Core::Maths;
using namespace Engine::Core::Maths::Shape3D;
using namespace Engine::Core::Maths::ShapeRelation;

bool SegmentPlane::isSegmentPlaneCollided(const Segment& seg, const Plane& plane, Intersection& intersection)
{
    Vec3 AB = seg.getPt2() - seg.getPt1();
    float sub = Vec3::dot(AB, plane.getNormal());
    float distanceSegToOrigin = Vec3::dot(seg.getPt1(), plane.getNormal());

    /*if(sub == 0) segmen is orthogonal to the plan*/
    if (std::abs(sub) <= std::numeric_limits<float>::epsilon())
    {
        /*Check if segmen is confused with the plan*/
        /*Compute the distance of the segment with origin and compare with the distance of plan with origin*/

        intersection.setInifitIntersection();
        return std::abs(distanceSegToOrigin - plane.getDistance()) <= std::numeric_limits<float>::epsilon();
    }

    /*(n . PO( or d ) - n.OA) / n . AB*/
    float t = ((plane.getDistance() - distanceSegToOrigin) / sub);

    /*if t is not between 0 and 1 so not collision else compute OM with OM = OA + t * AB*/
    if (t < 0.f || t > 1.f)
    {
        intersection.setNotIntersection();
        return false;
    }
    else
    {
        intersection.setOneIntersection(seg.getPt1() + t * AB);

        if (distanceSegToOrigin > plane.getDistance())
        {
            intersection.normalI1 = plane.getNormal();
        }
        else
        {
            intersection.normalI1 = -plane.getNormal();
        }

        return true;
    }
}