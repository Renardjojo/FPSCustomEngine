#include "GE/Core/Maths/ShapeRelation/SegmentCapsule.hpp"

#include "GE/Core/Maths/Shape3D/InfiniteCylinder.hpp"
#include "GE/Core/Maths/Shape3D/Plane.hpp"
#include "GE/Core/Maths/ShapeRelation/SegmentInfiniteCylinder.hpp"
#include "GE/Core/Maths/ShapeRelation/SegmentPlane.hpp"
#include "GE/Core/Maths/ShapeRelation/SegmentSphere.hpp"

#include <limits>

using namespace Engine::Core::Maths;
using namespace Engine::Core::Maths::Shape3D;
using namespace Engine::Core::Maths::ShapeRelation;

bool SegmentCapsule::isSegmentCapsuleCollided(const Segment& seg, const Capsule& capsule, Intersection& intersection)
{
    InfiniteCylinder infCyl = capsule.getInfiniteCylinder();

    /*Check if collision happend with the infite cylinder on the capsule*/
    if (!SegmentInfiniteCylinder::isSegmentInfiniteCylinderCollided(seg, infCyl, intersection))
    {
        return false;
    }

    /*Detect position with outCode*/
    int outCodePt1, outCodePt2;
    detectSegmentPointPosition(seg, capsule, outCodePt1, outCodePt2);

    /*If the both points of the segment is on the same midle zone of the cylinder return the infiniteCylinder Intersection*/
    if (outCodePt1 == INSIDE && outCodePt2 == INSIDE)
    {
        return true;
    }

    /*Check if the semgent's point are on the same side.*/
    if (((outCodePt1 & ON_THE_LEFT_MASK) == (outCodePt2 & ON_THE_LEFT_MASK)) && !(outCodePt1 == INSIDE || outCodePt2 == INSIDE ))
    {
        /*The points are on both position. Both on Left or both on right*/
        if (((outCodePt1 & ON_EXTERNAL_MASK) == (outCodePt2 & ON_EXTERNAL_MASK)))
        {
            if ((outCodePt1 & ON_EXTERNAL_MASK) == ON_EXTERNAL_MASK)
            {
                /*Both point are on external zone*/
                intersection.setNotIntersection();
                return false;
            }
            else
            {
                /*Both point are on internal zone. Check the position and test with circle position*/
                Sphere sphere = ((outCodePt1 & ON_THE_LEFT_MASK) == ON_THE_LEFT_MASK) ? capsule.LeftSphere() : capsule.RightSphere();
                return SegmentSphere::isSegmentSphereCollided(seg, sphere, intersection);
            }
        }
    }

    /*Check the intersection found on the infinyte cylinder and remove the wrong intersection*/
    checkCapsuleInfinitCylinderCollisionPoint(capsule, intersection);
    if (intersection.intersectionType == EIntersectionType::TwoIntersectiont)
        return true;

    /*Try all combination of position of point 1 with position of point 2 and check the associate collision detection*/
    if ((outCodePt1 & ON_THE_LEFT_MASK) == ON_THE_LEFT_MASK)
    {
        if ((outCodePt2 & ON_THE_RIGHT_MASK) == ON_THE_RIGHT_MASK)
        {

            checkLeftCapsuleSphereCollision(seg, capsule, intersection);

            if (intersection.intersectionType == EIntersectionType::TwoIntersectiont)
                return true;

            checkRightCapsuleSphereCollision(seg, capsule, intersection);

            return intersection.intersectionType != EIntersectionType::NoIntersection;
        }
        else //On the middle or on the left
        {
            checkLeftCapsuleSphereCollision(seg, capsule, intersection);
            return intersection.intersectionType != EIntersectionType::NoIntersection;
        }
    }
    else if ((outCodePt1 & ON_THE_RIGHT_MASK) == ON_THE_RIGHT_MASK)
    {
        if ((outCodePt2 & ON_THE_LEFT_MASK) == ON_THE_LEFT_MASK)
        {
                return true;

            checkLeftCapsuleSphereCollision(seg, capsule, intersection);

            return intersection.intersectionType != EIntersectionType::NoIntersection;
        }
        else //On the middle or on the left
        {
            checkRightCapsuleSphereCollision(seg, capsule, intersection);
            return intersection.intersectionType != EIntersectionType::NoIntersection;
        }
    }
    else //On the middle
    {
        if ((outCodePt2 & ON_THE_LEFT_MASK) == ON_THE_LEFT_MASK)
        {
            checkLeftCapsuleSphereCollision(seg, capsule, intersection);
            return intersection.intersectionType != EIntersectionType::NoIntersection;
        }
        else //On the middle or on the right
        {
            checkRightCapsuleSphereCollision(seg, capsule, intersection);
            return intersection.intersectionType != EIntersectionType::NoIntersection;
        }
    }
}

void SegmentCapsule::detectSegmentPointPosition(const Segment& seg, const Capsule& capsule, int& outCodePt1, int& outCodePt2)
{
    Plane leftCylindreFace = capsule.BodyCylinder().LeftPlane();
    Plane rightCylindreFace = capsule.BodyCylinder().RightPlane();

    float pt1DistToLeftInternalFace = leftCylindreFace.getSignedDistanceToPlane(seg.getPt1());
    float pt2DistToLeftInternalFace = leftCylindreFace.getSignedDistanceToPlane(seg.getPt2());
    float pt1DistToRightInternalFace = rightCylindreFace.getSignedDistanceToPlane(seg.getPt1());
    float pt2DistToRightInternalFace = rightCylindreFace.getSignedDistanceToPlane(seg.getPt2());

    Vec3 normalLeftFace = (capsule.getSegment().getPt1() - capsule.getSegment().getPt2()).normalize();
    Plane leftCapsuleFace = {capsule.getSegment().getPt1() + normalLeftFace * capsule.getRadius(), normalLeftFace};
    Sphere leftSphere = capsule.LeftSphere();

    Plane rightCapsuleFace = {capsule.getSegment().getPt2() + (-normalLeftFace * capsule.getRadius()), -normalLeftFace};
    Sphere rightSphere = capsule.RightSphere();

    float pt1DistToLeftExternalFace = leftCapsuleFace.getSignedDistanceToPlane(seg.getPt1());
    float pt2DistToLeftExternalFace = leftCapsuleFace.getSignedDistanceToPlane(seg.getPt2());
    float pt1DistToRightExternalFace = rightCapsuleFace.getSignedDistanceToPlane(seg.getPt1());
    float pt2DistToRightExternalFace = rightCapsuleFace.getSignedDistanceToPlane(seg.getPt2());

    if (pt1DistToLeftInternalFace >= std::numeric_limits<float>::epsilon())
    {
        outCodePt1 = LEFT_INTERNAL;

        if (pt1DistToLeftExternalFace >= std::numeric_limits<float>::epsilon())
        {
            
            outCodePt1 = LEFT_EXTERNAL;
        }
    }
    else if (pt1DistToRightInternalFace >= std::numeric_limits<float>::epsilon())
    {
        outCodePt1 = RIGHT_INTERNAL;

        if (pt1DistToRightExternalFace >= std::numeric_limits<float>::epsilon())
        {
            outCodePt1 = RIGHT_EXTERNAL;
        }
    }
    else
    {
        outCodePt1 = INSIDE;
    }

    if (pt2DistToLeftInternalFace >= std::numeric_limits<float>::epsilon())
    {
        outCodePt2 = LEFT_INTERNAL;

        if (pt2DistToLeftExternalFace >= std::numeric_limits<float>::epsilon())
        {
            outCodePt2 = LEFT_EXTERNAL;
        }
    }
    else if (pt2DistToRightInternalFace >= std::numeric_limits<float>::epsilon())
    {
        outCodePt2 = RIGHT_INTERNAL;

        if (pt2DistToRightExternalFace >= std::numeric_limits<float>::epsilon())
        {
            outCodePt2 = RIGHT_EXTERNAL;
        }
    }
    else
    {
        outCodePt2 = INSIDE;
    }
}

void SegmentCapsule::checkCapsuleInfinitCylinderCollisionPoint(const Capsule& capsule, Intersection& intersection)
{
    if (intersection.intersectionType == EIntersectionType::OneIntersectiont)
    {
        if (pointIsBetweenCapsuleSegLimit(capsule, intersection.intersection1))
        {
            return;
        }
        else
        {
            intersection.setNotIntersection();
        }
    }
    else if (intersection.intersectionType == EIntersectionType::TwoIntersectiont)
    {
        bool keepInter1 = false;
        bool keepInter2 = false;

        /*Check if intersectio 1 and 2 is on the capsule*/
        if (pointIsBetweenCapsuleSegLimit(capsule, intersection.intersection1))
        {
            keepInter1 = true;
        }

        if (pointIsBetweenCapsuleSegLimit(capsule, intersection.intersection2))
        {
            keepInter2 = true;
        }

        /*processes the test result*/
        if (keepInter1)
        {
            if (!keepInter2)
            {
                intersection.intersectionType = EIntersectionType::OneIntersectiont;
            }
        }
        else if (keepInter2)
        {
            intersection.setOneIntersection(intersection.intersection2);
            intersection.normalI1 = intersection.normalI2;
        }
        else
        {
            intersection.setNotIntersection();
        }
    }
}

void SegmentCapsule::checkLeftCapsuleSphereCollision (const Segment& seg, const Capsule& capsule, Intersection& intersection)
{
    Sphere leftCapsuleSphere = capsule.LeftSphere();

    Intersection shapeIntersection;
    if (SegmentSphere::isSegmentSphereCollided(seg, leftCapsuleSphere, shapeIntersection))
    {
        if (shapeIntersection.intersectionType == EIntersectionType::OneIntersectiont)
        {
            if (capsule.getSegment().getLeftPlane().getSignedDistanceToPlane(shapeIntersection.intersection1) >= std::numeric_limits<float>::epsilon())
            {
                if (intersection.intersectionType == EIntersectionType::OneIntersectiont)
                {
                    intersection.setSecondIntersection(shapeIntersection.intersection1);
                    intersection.normalI2 = shapeIntersection.normalI1;
                }
                else
                {
                    intersection.setOneIntersection(shapeIntersection.intersection1);
                    intersection.normalI1 = shapeIntersection.normalI1;
                }
            }
        }
        else if (shapeIntersection.intersectionType == EIntersectionType::TwoIntersectiont)
        {
            bool keepInter1 = false;
            bool keepInter2 = false;

            /*Check if intersectio 1 and 2 is on the capsule*/
            if (capsule.getSegment().getLeftPlane().getSignedDistanceToPlane(shapeIntersection.intersection1) >= std::numeric_limits<float>::epsilon())
            {
                keepInter1 = true;
            }

            if (capsule.getSegment().getLeftPlane().getSignedDistanceToPlane(shapeIntersection.intersection2) >= std::numeric_limits<float>::epsilon())
            {
                keepInter2 = true;
            }

            /*processes the test result*/
            if (keepInter1)
            {
                if (intersection.intersectionType == EIntersectionType::OneIntersectiont)
                {
                    intersection.setSecondIntersection(shapeIntersection.intersection1);
                    intersection.normalI2 = shapeIntersection.normalI1;
                    intersection.sortIntersection(seg.getPt1());
    
                    return;
                }
                else
                {
                    intersection.setOneIntersection(shapeIntersection.intersection1);
                    intersection.normalI1 = shapeIntersection.normalI1;
                }

                if (keepInter2)
                {
                    intersection.setSecondIntersection(shapeIntersection.intersection2);
                    intersection.normalI2 = shapeIntersection.normalI2;
                }
            }
            else if (keepInter2)
            {
                if (intersection.intersectionType == EIntersectionType::OneIntersectiont)
                {
                    intersection.setSecondIntersection(shapeIntersection.intersection2);
                    intersection.normalI2 = shapeIntersection.normalI2;
                }
                else
                {
                    intersection.setOneIntersection(shapeIntersection.intersection2);
                    intersection.normalI1 = shapeIntersection.normalI2;
                }
            }

            intersection.sortIntersection(seg.getPt1());
        }
    }
}

void SegmentCapsule::checkRightCapsuleSphereCollision(const Segment& seg, const Capsule& capsule, Intersection& intersection)
{
    Sphere rightCapsuleSphere = capsule.RightSphere();

    Intersection shapeIntersection;
    if (SegmentSphere::isSegmentSphereCollided(seg, rightCapsuleSphere, shapeIntersection))
    {
        if (shapeIntersection.intersectionType == EIntersectionType::OneIntersectiont)
        {
            if (capsule.getSegment().getRightPlane().getSignedDistanceToPlane(shapeIntersection.intersection1) >= std::numeric_limits<float>::epsilon())
            {
                if (intersection.intersectionType == EIntersectionType::OneIntersectiont)
                {
                    intersection.setSecondIntersection(shapeIntersection.intersection1);
                    intersection.normalI2 = shapeIntersection.normalI1;
                }
                else
                {
                    intersection.setOneIntersection(shapeIntersection.intersection1);
                    intersection.normalI1 = shapeIntersection.normalI1;
                }
            }
        }
        else if (shapeIntersection.intersectionType == EIntersectionType::TwoIntersectiont)
        {
            bool keepInter1 = false;
            bool keepInter2 = false;

            /*Check if intersectio 1 and 2 is on the capsule*/
            if (capsule.getSegment().getRightPlane().getSignedDistanceToPlane(shapeIntersection.intersection1) >= std::numeric_limits<float>::epsilon())
            {
                keepInter1 = true;
            }

            if (capsule.getSegment().getRightPlane().getSignedDistanceToPlane(shapeIntersection.intersection2) >= std::numeric_limits<float>::epsilon())
            {
                keepInter2 = true;
            }

            /*processes the test result*/
            if (keepInter1)
            {
                if (intersection.intersectionType == EIntersectionType::OneIntersectiont)
                {
                    intersection.setSecondIntersection(shapeIntersection.intersection1);
                    intersection.normalI2 = shapeIntersection.normalI1;
                    intersection.sortIntersection(seg.getPt1());
                    return;
                }
                else
                {
                    intersection.setOneIntersection(shapeIntersection.intersection1);
                    intersection.normalI1 = shapeIntersection.normalI1;
                }

                if (keepInter2)
                {
                    intersection.setSecondIntersection(shapeIntersection.intersection2);
                    intersection.normalI2 = shapeIntersection.normalI2;
                }
            }
            else if (keepInter2)
            {
                if (intersection.intersectionType == EIntersectionType::OneIntersectiont)
                {
                    intersection.setSecondIntersection(shapeIntersection.intersection2);
                    intersection.normalI2 = shapeIntersection.normalI2;
                }
                else
                {
                    intersection.setOneIntersection(shapeIntersection.intersection2);
                    intersection.normalI1 = shapeIntersection.normalI2;
                }
            }
            intersection.sortIntersection(seg.getPt1());
        }
    }
}

bool SegmentCapsule::pointIsBetweenCapsuleSegLimit(const Capsule& capsule, const Vec3& pt)
{
    //Binary optimisation. Avoid AND operator
    return !(capsule.getSegment().getLeftPlane().getSignedDistanceToPlane(pt) > std::numeric_limits<float>::epsilon() || capsule.getSegment().getRightPlane().getSignedDistanceToPlane(pt) > std::numeric_limits<float>::epsilon());
}