#include "GE/Core/Maths/ShapeRelation/SegmentInfiniteCylinder.hpp"

#include "GE/Core/Maths/vec.hpp"

using namespace Engine::Core::Maths;
using namespace Engine::Core::Maths::Shape3D;
using namespace Engine::Core::Maths::ShapeRelation;

bool SegmentInfiniteCylinder::isSegmentInfiniteCylinderCollided(const Segment& seg, const InfiniteCylinder& infCylinder, Intersection& intersection)
{ 
    Vec3 OQ  = infCylinder.getLine().getOrigin() + infCylinder.getLine().getNormal();
    Vec3 AB  = seg.getPt2() - seg.getPt1();
    Vec3 PO  = -infCylinder.getLine().getOrigin();
    Vec3 OA  = seg.getPt1();
    float R     = infCylinder.getRadius();

    Vec3 vecEq1 = Vec3::cross(OQ, OA + PO) + Vec3::cross(PO, OA);
    Vec3 vecEq2 = Vec3::cross((OQ + PO), AB);
    float magnitudEq1 = vecEq1.length();
    float magnitudEq2 = vecEq2.length();
    
    float magnitudSquareEq1 = magnitudEq1 * magnitudEq1;
    float magnitudSquareEq2 = magnitudEq2 * magnitudEq2;
    
    float a = magnitudSquareEq2;
    float b = 2.f * Vec3::dot(vecEq1, vecEq2);
    float c = magnitudSquareEq1 - R * R;

    /*We comput the discriminent*/
    if (Intersection::computeDiscriminentAndSolveEquation(a, b, c, seg.getPt1(), seg.getPt2(), intersection))
    {
        if (intersection.intersectionType == EIntersectionType::TwoIntersectiont)
        {
            Vec3 cylPtToInter2 = intersection.intersection2 - infCylinder.getLine().getOrigin();
            intersection.normalI2 = (cylPtToInter2 - infCylinder.getLine().getNormal() * Vec3::dot(cylPtToInter2, infCylinder.getLine().getNormal())).normalize();
        }
        Vec3 cylPtToInter1 = intersection.intersection1 - infCylinder.getLine().getOrigin();
        intersection.normalI1 = (cylPtToInter1 - infCylinder.getLine().getNormal() * Vec3::dot(cylPtToInter1, infCylinder.getLine().getNormal())).normalize();

        return true;
    }
    else
    {
        return false;
    }
}