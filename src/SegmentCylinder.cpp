#include "GE/Core/Maths/ShapeRelation/SegmentCylinder.hpp"

#include "GE/Core/Maths/Shape3D/InfiniteCylinder.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/Shape3D/Plane.hpp"
#include "GE/Core/Maths/ShapeRelation/SegmentInfiniteCylinder.hpp"
#include "GE/Core/Maths/ShapeRelation/SegmentPlane.hpp"

#include <limits>

using namespace Engine::Core::Maths;
using namespace Engine::Core::Maths::Shape3D;
using namespace Engine::Core::Maths::ShapeRelation;

bool SegmentCylinder::isSegmentCylinderCollided(const Segment& seg, const Cylinder& cylinder, Intersection& intersection)
{
    InfiniteCylinder infinitCyl = cylinder.getInfiniteCylinder();

    /*If there not have collision this infinit cylindre coaxile with the cylindre return false*/
    if (!SegmentInfiniteCylinder::isSegmentInfiniteCylinderCollided(seg, infinitCyl, intersection))
    {
        intersection.setNotIntersection();
        return false;
    }

    /*If the both points of the segment is on the same midle zone of the cylinder return the infiniteCylinder Intersection*/
    Plane leftCylindreFace = cylinder.LeftPlane();
    Plane rightCylindreFace = cylinder.RightPlane();

    bool pt1InFrontOfLeftFace = leftCylindreFace.getSignedDistanceToPlane(seg.getPt1()) > std::numeric_limits<float>::epsilon();
    bool pt1InFrontOfRightFace = rightCylindreFace.getSignedDistanceToPlane(seg.getPt1()) > std::numeric_limits<float>::epsilon();
    bool pt2InFrontOfLeftFace = leftCylindreFace.getSignedDistanceToPlane(seg.getPt2()) > std::numeric_limits<float>::epsilon();
    bool pt2InFrontOfRightFace = rightCylindreFace.getSignedDistanceToPlane(seg.getPt2()) > std::numeric_limits<float>::epsilon();

    if (!pt1InFrontOfLeftFace && !pt1InFrontOfRightFace && !pt2InFrontOfLeftFace && !pt2InFrontOfRightFace)
    {
        return true;
    }

    /*If the both points of the segment is on the same side return false*/
    if ((pt1InFrontOfLeftFace && pt2InFrontOfLeftFace) || (pt1InFrontOfRightFace && pt2InFrontOfRightFace))
    {
        intersection.setNotIntersection();
        return false;
    }

    /*There is one intersection with the infinite cylinder*/
    if (intersection.intersectionType == EIntersectionType::OneIntersectiont)
    {
        /*Check if the intersection point is inside the cylinder*/
        if (leftCylindreFace.getSignedDistanceToPlane(intersection.intersection1) > std::numeric_limits<float>::epsilon())
        {
            Intersection segQuadIntersection;

            if (SegmentPlane::isSegmentPlaneCollided(seg, leftCylindreFace, segQuadIntersection))
            {
                if (infinitCyl.isPointInside(segQuadIntersection.intersection1))
                {
                    intersection.intersection1 = segQuadIntersection.intersection1;
                    intersection.normalI1 = pt1InFrontOfLeftFace ? segQuadIntersection.normalI1 : -segQuadIntersection.normalI1;

                    /*Check if there are a second intersection on the other face*/
                    if (SegmentPlane::isSegmentPlaneCollided(seg, rightCylindreFace, segQuadIntersection))
                    {
                        if (infinitCyl.isPointInside(segQuadIntersection.intersection1))
                        {
                            intersection.intersection2 = segQuadIntersection.intersection1;
                            intersection.normalI2 = pt1InFrontOfLeftFace ? -segQuadIntersection.normalI1 : segQuadIntersection.normalI1;
                            intersection.intersectionType = EIntersectionType::TwoIntersectiont;
                        }
                    }
                    intersection.sortIntersection(seg.getPt1());
                    return true;
                }
            }

            intersection.setNotIntersection();
            return false;
        }
        else if (rightCylindreFace.getSignedDistanceToPlane(intersection.intersection1) > std::numeric_limits<float>::epsilon())
        {
            Intersection segQuadIntersection;

            if (SegmentPlane::isSegmentPlaneCollided(seg, rightCylindreFace, segQuadIntersection))
            {
                if (infinitCyl.isPointInside(segQuadIntersection.intersection1))
                {
                    intersection.intersection1 = segQuadIntersection.intersection1;
                    intersection.normalI1 = pt1InFrontOfRightFace ? segQuadIntersection.normalI1 : -segQuadIntersection.normalI1;

                    /*Check if there are a second intersection on the other face*/
                    if (SegmentPlane::isSegmentPlaneCollided(seg, leftCylindreFace, segQuadIntersection))
                    {
                        if (infinitCyl.isPointInside(segQuadIntersection.intersection1))
                        {
                            intersection.intersection2 = segQuadIntersection.intersection1;
                            intersection.normalI2 = pt1InFrontOfRightFace ? -segQuadIntersection.normalI1 : segQuadIntersection.normalI1;
                            intersection.intersectionType = EIntersectionType::TwoIntersectiont;
                        }
                    }
                    intersection.sortIntersection(seg.getPt1());
                    return true;
                }
            }
            intersection.setNotIntersection();
            return false;
        }
        else
        {
            Intersection segQuadIntersection;
            if (SegmentPlane::isSegmentPlaneCollided(seg, leftCylindreFace, segQuadIntersection))
            {
                if (infinitCyl.isPointInside(segQuadIntersection.intersection1))
                {
                    intersection.intersection2 = segQuadIntersection.intersection1;
                    intersection.normalI2 = pt1InFrontOfLeftFace ? segQuadIntersection.normalI1 : -segQuadIntersection.normalI1;
                    intersection.intersectionType = EIntersectionType::TwoIntersectiont;
                    
                    intersection.sortIntersection(seg.getPt1());
                    return true;
                }
            }

            if (SegmentPlane::isSegmentPlaneCollided(seg, rightCylindreFace, segQuadIntersection))
            {
                if (infinitCyl.isPointInside(segQuadIntersection.intersection1))
                {
                    intersection.intersection2 = segQuadIntersection.intersection1;
                    intersection.normalI2 = pt1InFrontOfRightFace ? segQuadIntersection.normalI1 : -segQuadIntersection.normalI1;
                    intersection.intersectionType = EIntersectionType::TwoIntersectiont;
                    
                    intersection.sortIntersection(seg.getPt1());
                    return true;
                }
            }
            intersection.sortIntersection(seg.getPt1());
            return true;
        }
    }

    /*There is two intersection with the infinite cylinder*/
    if (intersection.intersectionType == EIntersectionType::TwoIntersectiont)
    {
        bool keepInter1 = false;
        bool keepInter2 = false;

        /*if intersection 1 is not inside the cylindre*/
        if (leftCylindreFace.getSignedDistanceToPlane(intersection.intersection1) > std::numeric_limits<float>::epsilon())
        {
            Intersection segQuadIntersection;

            if (SegmentPlane::isSegmentPlaneCollided(seg, leftCylindreFace, segQuadIntersection))
            {
                if (infinitCyl.isPointInside(segQuadIntersection.intersection1))
                {
                    intersection.intersection1 = segQuadIntersection.intersection1;
                    intersection.normalI1 = pt1InFrontOfLeftFace ? segQuadIntersection.normalI1 : -segQuadIntersection.normalI1;
                    keepInter1 = true;
                }
            }
        }
        else if (rightCylindreFace.getSignedDistanceToPlane(intersection.intersection1) > std::numeric_limits<float>::epsilon())
        {
            Intersection segQuadIntersection;
            if (SegmentPlane::isSegmentPlaneCollided(seg, rightCylindreFace, segQuadIntersection))
            {
                if (infinitCyl.isPointInside(segQuadIntersection.intersection1))
                {
                    intersection.intersection1 = segQuadIntersection.intersection1;
                    intersection.normalI1 = pt1InFrontOfRightFace ? segQuadIntersection.normalI1 : -segQuadIntersection.normalI1;
                    keepInter1 = true;
                }
            }
        }
        else
        {
            keepInter1 = true;
        }

        /*if the intersection both is not inside the cylindre*/
        if (leftCylindreFace.getSignedDistanceToPlane(intersection.intersection2) > std::numeric_limits<float>::epsilon())
        {
            Intersection segQuadIntersection;
            if (SegmentPlane::isSegmentPlaneCollided(seg, leftCylindreFace, segQuadIntersection))
            {
                if (infinitCyl.isPointInside(segQuadIntersection.intersection1))
                {
                    intersection.intersection2 = segQuadIntersection.intersection1;
                    intersection.normalI2 = pt1InFrontOfLeftFace ? segQuadIntersection.normalI1 : -segQuadIntersection.normalI1;
                    keepInter2 = true;
                }
            }
        }
        else if (rightCylindreFace.getSignedDistanceToPlane(intersection.intersection2) > std::numeric_limits<float>::epsilon())
        {
            Intersection segQuadIntersection;
            if (SegmentPlane::isSegmentPlaneCollided(seg, rightCylindreFace, segQuadIntersection))
            {
                if (infinitCyl.isPointInside(segQuadIntersection.intersection1))
                {
                    intersection.intersection2 = segQuadIntersection.intersection1;
                    intersection.normalI2 = pt1InFrontOfRightFace ? segQuadIntersection.normalI1 : -segQuadIntersection.normalI1;
                    keepInter2 = true;
                }
            }
        }
        else
        {
            keepInter2 = true;
        }

        if (keepInter1)
        {
            if (keepInter2)
            {
                intersection.sortIntersection(seg.getPt1());
                return true;
            }
            else
            {
                intersection.intersectionType = EIntersectionType::OneIntersectiont;
                return true;
            }
        }
        else if (keepInter2)
        {
            intersection.setOneIntersection(intersection.intersection2);
            return true;
        }
        else
        {
            intersection.setNotIntersection();
            return false;
        }
    }

    /*The point is on the infiniteCylinder. Not inside the cylinder and note nd the same side. So, Check if there is 1 or 2 collision*/
    if (pt1InFrontOfLeftFace && pt2InFrontOfRightFace)
    {
        Intersection segQuadIntersection;
        SegmentPlane::isSegmentPlaneCollided(seg, leftCylindreFace, segQuadIntersection);
        intersection.intersection1 = segQuadIntersection.intersection1;
        intersection.normalI1 = pt1InFrontOfLeftFace ? segQuadIntersection.normalI1 : -segQuadIntersection.normalI1;

        SegmentPlane::isSegmentPlaneCollided(seg, rightCylindreFace, segQuadIntersection);
        intersection.intersection2 = segQuadIntersection.intersection1;
        intersection.normalI2 = pt1InFrontOfLeftFace ? -segQuadIntersection.normalI1 : segQuadIntersection.normalI1;

        intersection.intersectionType = EIntersectionType::TwoIntersectiont;
        return true;
    }
    else if (pt1InFrontOfRightFace && pt2InFrontOfLeftFace)
    {
        Intersection segQuadIntersection;
        SegmentPlane::isSegmentPlaneCollided(seg, rightCylindreFace, segQuadIntersection);
        intersection.intersection1 = segQuadIntersection.intersection1;
        intersection.normalI1 = pt1InFrontOfRightFace ? segQuadIntersection.normalI1 : -segQuadIntersection.normalI1;

        SegmentPlane::isSegmentPlaneCollided(seg, leftCylindreFace, segQuadIntersection);
        intersection.intersection2 = segQuadIntersection.intersection1;
        intersection.normalI2 = pt1InFrontOfRightFace ? -segQuadIntersection.normalI1 : segQuadIntersection.normalI1;

        intersection.intersectionType = EIntersectionType::TwoIntersectiont;
        return true;
    }
    else //pt1 or pt2 is inside
    {
        if ((!pt1InFrontOfRightFace && pt2InFrontOfRightFace)|| (pt1InFrontOfRightFace && !pt2InFrontOfRightFace))
        {
            Intersection segQuadIntersection;
            SegmentPlane::isSegmentPlaneCollided(seg, rightCylindreFace, segQuadIntersection);
            intersection.setOneIntersection(segQuadIntersection.intersection1);
            intersection.normalI1 = pt1InFrontOfRightFace ? segQuadIntersection.normalI1 : -segQuadIntersection.normalI1;
        }
        else
        {
            Intersection segQuadIntersection;
            SegmentPlane::isSegmentPlaneCollided(seg, leftCylindreFace, segQuadIntersection);
            intersection.setOneIntersection(segQuadIntersection.intersection1);
            intersection.normalI1 = pt1InFrontOfLeftFace ? segQuadIntersection.normalI1 : -segQuadIntersection.normalI1;
        }
        return true;
    }
}