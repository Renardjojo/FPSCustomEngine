#include "GE/Core/Maths/ShapeRelation/MovingSphereOrientedBox.hpp"

#include "GE/Core/Maths/ShapeRelation/SegmentOrientedBox.hpp"
#include "GE/Core/Maths/ShapeRelation/SegmentCapsule.hpp"
#include "GE/Core/Maths/Referential.hpp"

using namespace Engine::Core::Maths;
using namespace Engine::Core::Maths::Shape3D;
using namespace Engine::Core::Maths::ShapeRelation;

/*get the first collision point between moving sphere and box*/
bool MovingSphereOrientedBox::isMovingSphereOrientedBoxCollided(const Sphere& sphere, const OrientedBox& box, const Vec3& sphereVelocity, Intersection& intersection)
{
    OrientedBox minkowskiSumOBB = getMinkowskiSumOBB(box, sphere.getRadius());
    Segment spherePt1ToPt2 {sphere.getCenter(), sphere.getCenter() + sphereVelocity};

    /*Step 1, check if the segment collid with the Minkowski sum box*/
    if (!SegmentOrientedBox::isSegmentOrientedBoxCollided(spherePt1ToPt2, minkowskiSumOBB, intersection))
    {
        intersection.setNotIntersection();
        return false;
    }

    /*Step 2, check if intersection points are on the veronoi face*/
    applyVeronoiRegionCorrection(box, intersection, spherePt1ToPt2, sphere.getRadius());

    return intersection.intersectionType != EIntersectionType::NoIntersection;
}

OrientedBox MovingSphereOrientedBox::getMinkowskiSumOBB (const OrientedBox& box, float sphereRadius)
{
    return OrientedBox{box.getReferential(), box.getExtI() + sphereRadius, box.getExtJ() + sphereRadius, box.getExtK() + sphereRadius};
}

void MovingSphereOrientedBox::applyVeronoiRegionCorrection(const OrientedBox& box, Intersection& intersection, const Segment& seg, float sphereRadius)
{
    if (intersection.intersectionType == EIntersectionType::OneIntersectiont)
    {
        int topVeronoiOutCode = getTopVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(intersection.intersection1);
        int rightVeronoiOutCode = getRightVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(intersection.intersection1);
        int forwardVeronoiOutCode = getFowardVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(intersection.intersection1);

        if (topVeronoiOutCode == QUAD_OUTCODE_INSIDE || rightVeronoiOutCode == QUAD_OUTCODE_INSIDE || forwardVeronoiOutCode == QUAD_OUTCODE_INSIDE)
        {
            return;
        }

        /*Define if AB or BA in function of the position of the first segment point. If This point is inside do BA else AB*/
        int topVeronoiOutCodeSeg1 = getTopVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(seg.getPt1());
        int rightVeronoiOutCodeSeg1 = getRightVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(seg.getPt1());
        int forwardVeronoiOutCodeSeg1 = getFowardVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(seg.getPt1());

        if (topVeronoiOutCodeSeg1 == QUAD_OUTCODE_INSIDE && rightVeronoiOutCodeSeg1 == QUAD_OUTCODE_INSIDE && forwardVeronoiOutCodeSeg1 == QUAD_OUTCODE_INSIDE)
        {
            applyVeronoiRegionCorrectionWithOutCode(box, intersection, Segment{seg.getPt2(), seg.getPt1()}, sphereRadius, topVeronoiOutCode, rightVeronoiOutCode, forwardVeronoiOutCode, true);
        }
        else
        {
            applyVeronoiRegionCorrectionWithOutCode(box, intersection, seg, sphereRadius, topVeronoiOutCode, rightVeronoiOutCode, forwardVeronoiOutCode, true);
        }
    }
    else if (intersection.intersectionType == EIntersectionType::TwoIntersectiont)
    {
        bool keepInter1 = false;
        bool keepInter2 = false;

        int topVeronoiOutCodePt1 = getTopVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(intersection.intersection1);
        int rightVeronoiOutCodePt1 = getRightVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(intersection.intersection1);
        int forwardVeronoiOutCodePt1 = getFowardVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(intersection.intersection1);

        int topVeronoiOutCodePt2 = getTopVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(intersection.intersection2);
        int rightVeronoiOutCodePt2 = getRightVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(intersection.intersection2);
        int forwardVeronoiOutCodePt2 = getBackwardVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(intersection.intersection2);

        if (topVeronoiOutCodePt1 == QUAD_OUTCODE_INSIDE || rightVeronoiOutCodePt1 == QUAD_OUTCODE_INSIDE || forwardVeronoiOutCodePt1 == QUAD_OUTCODE_INSIDE)
        {
            keepInter1 = true;
        }

        if (topVeronoiOutCodePt2 == QUAD_OUTCODE_INSIDE || rightVeronoiOutCodePt2 == QUAD_OUTCODE_INSIDE || forwardVeronoiOutCodePt2 == QUAD_OUTCODE_INSIDE)
        {
            keepInter2 = true;
        }

        if (keepInter1)
        {
            if (!keepInter2)
            {
                applyVeronoiRegionCorrectionWithOutCode(box, intersection, seg, sphereRadius, topVeronoiOutCodePt2, rightVeronoiOutCodePt2, forwardVeronoiOutCodePt2, false);
            }
        }
        else if (keepInter2)
        {
            if (!keepInter1)
            {
                applyVeronoiRegionCorrectionWithOutCode(box, intersection, seg, sphereRadius, topVeronoiOutCodePt1, rightVeronoiOutCodePt1, forwardVeronoiOutCodePt1, true);
            }
        }
        else 
        {
            applyVeronoiRegionCorrectionWithOutCode(box, intersection, seg, sphereRadius, topVeronoiOutCodePt2, rightVeronoiOutCodePt2, forwardVeronoiOutCodePt2, false);
            applyVeronoiRegionCorrectionWithOutCode(box, intersection, seg, sphereRadius, topVeronoiOutCodePt1, rightVeronoiOutCodePt1, forwardVeronoiOutCodePt1, true);
        }
    } 
    else if (intersection.intersectionType == EIntersectionType::InfinyIntersection)
    {
        int topVeronoiOutCodeSegPt1 = getTopVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(seg.getPt1());
        int rightVeronoiOutCodeSegPt1 = getRightVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(seg.getPt1());
        int forwardVeronoiOutCodeSegPt1 = getFowardVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(seg.getPt1());

        if (!(topVeronoiOutCodeSegPt1 == QUAD_OUTCODE_INSIDE || rightVeronoiOutCodeSegPt1 == QUAD_OUTCODE_INSIDE || forwardVeronoiOutCodeSegPt1 == QUAD_OUTCODE_INSIDE))
        {
            if (applyVeronoiRegionCorrectionWithOutCode(box, intersection, seg, sphereRadius, topVeronoiOutCodeSegPt1, rightVeronoiOutCodeSegPt1, forwardVeronoiOutCodeSegPt1, true))
            {
                intersection.intersectionType = EIntersectionType::OneIntersectiont;
            }
            else
            {
                intersection.intersectionType = EIntersectionType::NoIntersection;
            }
        }
    }     
}

bool MovingSphereOrientedBox::applyVeronoiRegionCorrectionWithOutCode(const OrientedBox& box, Intersection& intersection, const Segment& seg, float sphereRadius, int topOutCode, int rightOutCode, int forwardOutCode, bool checkFirstIntersection)
{
    if (topOutCode == QUAD_OUTCODE_INSIDE || rightOutCode == QUAD_OUTCODE_INSIDE || forwardOutCode == QUAD_OUTCODE_INSIDE)
    {
        return true;
    }

    if ((forwardOutCode & QUAD_OUTCODE_TOP) == QUAD_OUTCODE_TOP)
    {
        if ((topOutCode & QUAD_OUTCODE_TOP) == QUAD_OUTCODE_TOP)
        {
            return applyCapsuleCorrection(seg, getTopForwardVeronoiCapsule(box, sphereRadius), intersection, checkFirstIntersection);
        }
        else if ((topOutCode & QUAD_OUTCODE_BOTTOM) == QUAD_OUTCODE_BOTTOM)
        {
            return applyCapsuleCorrection(seg, getTopBackwardVeronoiCapsule(box, sphereRadius), intersection, checkFirstIntersection);
        }
        else if ((topOutCode & QUAD_OUTCODE_LEFT) == QUAD_OUTCODE_LEFT)
        {
            return applyCapsuleCorrection(seg, getTopLeftVeronoiCapsule(box, sphereRadius), intersection, checkFirstIntersection);
        }
        else
        {               
            return applyCapsuleCorrection(seg, getTopRightVeronoiCapsule(box, sphereRadius), intersection, checkFirstIntersection);
        }
    }
    else if ((forwardOutCode& QUAD_OUTCODE_BOTTOM) == QUAD_OUTCODE_BOTTOM)
    {
        if ((topOutCode & QUAD_OUTCODE_TOP) == QUAD_OUTCODE_TOP)
        {
            return applyCapsuleCorrection(seg, getBottomForwardVeronoiCapsule(box, sphereRadius), intersection, checkFirstIntersection);
        }
        else if ((topOutCode & QUAD_OUTCODE_BOTTOM) == QUAD_OUTCODE_BOTTOM)
        {
            return applyCapsuleCorrection(seg, getBottomBackwardVeronoiCapsule(box, sphereRadius), intersection, checkFirstIntersection);
        }
        else if ((topOutCode & QUAD_OUTCODE_LEFT) == QUAD_OUTCODE_LEFT)
        {
            return applyCapsuleCorrection(seg, getBottomLeftVeronoiCapsule(box, sphereRadius), intersection, checkFirstIntersection);
        }
        else
        {
            return applyCapsuleCorrection(seg, getBottomRightVeronoiCapsule(box, sphereRadius), intersection, checkFirstIntersection);
        }
    }
    else /*On the center*/
    {
        if ((topOutCode & (QUAD_OUTCODE_TOP | QUAD_OUTCODE_LEFT)) == QUAD_OUTCODE_TOP + QUAD_OUTCODE_LEFT)
        {
            return applyCapsuleCorrection(seg, getLeftForwardVeronoiCapsule(box, sphereRadius), intersection, checkFirstIntersection);
        }
        else if ((topOutCode & (QUAD_OUTCODE_TOP | QUAD_OUTCODE_RIGHT)) == QUAD_OUTCODE_TOP + QUAD_OUTCODE_RIGHT)
        {
            return applyCapsuleCorrection(seg, getRightForwardVeronoiCapsule(box, sphereRadius), intersection, checkFirstIntersection);
        }
        else if ((topOutCode & (QUAD_OUTCODE_BOTTOM | QUAD_OUTCODE_LEFT)) == QUAD_OUTCODE_BOTTOM + QUAD_OUTCODE_LEFT)
        {
            return applyCapsuleCorrection(seg, getLeftBackwardVeronoiCapsule(box, sphereRadius), intersection, checkFirstIntersection);
        }
        else
        {
            return applyCapsuleCorrection(seg, getRightBackwardVeronoiCapsule(box, sphereRadius), intersection, checkFirstIntersection);
        }
    }
}

bool MovingSphereOrientedBox::applyCapsuleCorrection(const Segment& seg, const Capsule& _capsule, Intersection& intersection, bool checkFirstIntersection)
{
    Intersection shapeIntersection;
    if (SegmentCapsule::isSegmentCapsuleCollided(seg, _capsule, shapeIntersection))
    {
        if (checkFirstIntersection)
        {
            intersection.intersection1 = shapeIntersection.intersection1;
            intersection.normalI1 = shapeIntersection.normalI1;
        }
        else
        {
            intersection.intersection2 = shapeIntersection.intersection2;
            intersection.normalI2 = shapeIntersection.normalI2;
        }
        return true;
    }
    else
    {
        if (checkFirstIntersection)
        {
            intersection.removeFirstIntersection();
        }
        else
        {
            intersection.removeSecondIntersection();
        }
    return false;
    }
}


Capsule MovingSphereOrientedBox::getTopLeftVeronoiCapsule(const OrientedBox& box, float sphereRadius)
{
    Vec3 capsuleCenter = box.getReferential().origin - box.getReferential().unitI * box.getExtI() + box.getReferential().unitJ * box.getExtJ();
    return Capsule{capsuleCenter, box.getReferential().unitK, box.getExtK() * 2.f, sphereRadius};
}

Capsule MovingSphereOrientedBox::getTopRightVeronoiCapsule(const OrientedBox& box, float sphereRadius)
{
    Vec3 capsuleCenter = box.getReferential().origin + box.getReferential().unitI * box.getExtI() + box.getReferential().unitJ * box.getExtJ();
        std::cout << capsuleCenter<< "  "   << box.getReferential().origin << std::endl;  
    return Capsule{capsuleCenter, box.getReferential().unitK, box.getExtK()* 2.f, sphereRadius};
}

Capsule MovingSphereOrientedBox::getTopForwardVeronoiCapsule(const OrientedBox& box, float sphereRadius)
{
    Vec3 capsuleCenter = box.getReferential().origin + box.getReferential().unitK * box.getExtK() + box.getReferential().unitJ * box.getExtJ();
    return Capsule{capsuleCenter, box.getReferential().unitI, box.getExtI() * 2.f, sphereRadius};
}

Capsule MovingSphereOrientedBox::getTopBackwardVeronoiCapsule(const OrientedBox& box, float sphereRadius)
{
    Vec3 capsuleCenter = box.getReferential().origin - box.getReferential().unitK * box.getExtK() + box.getReferential().unitJ * box.getExtJ();
    return Capsule{capsuleCenter, box.getReferential().unitI, box.getExtI() * 2.f, sphereRadius};
}

Capsule MovingSphereOrientedBox::getBottomLeftVeronoiCapsule(const OrientedBox& box, float sphereRadius)
{
    Vec3 capsuleCenter = box.getReferential().origin - box.getReferential().unitI * box.getExtI() - box.getReferential().unitJ * box.getExtJ();
    return Capsule{capsuleCenter, box.getReferential().unitK, box.getExtK() * 2.f, sphereRadius};
}

Capsule MovingSphereOrientedBox::getBottomRightVeronoiCapsule(const OrientedBox& box, float sphereRadius)
{
    Vec3 capsuleCenter = box.getReferential().origin + box.getReferential().unitI * box.getExtI() - box.getReferential().unitJ * box.getExtJ();
    return Capsule{capsuleCenter, box.getReferential().unitK, box.getExtK() * 2.f, sphereRadius};
}

Capsule MovingSphereOrientedBox::getBottomForwardVeronoiCapsule(const OrientedBox& box, float sphereRadius)
{
    Vec3 capsuleCenter = box.getReferential().origin + box.getReferential().unitK * box.getExtK() - box.getReferential().unitJ * box.getExtJ();
    return Capsule{capsuleCenter, box.getReferential().unitI, box.getExtI() * 2.f, sphereRadius};
}

Capsule MovingSphereOrientedBox::getBottomBackwardVeronoiCapsule(const OrientedBox& box, float sphereRadius)
{
    Vec3 capsuleCenter = box.getReferential().origin - box.getReferential().unitK * box.getExtK() - box.getReferential().unitJ * box.getExtJ();
    return Capsule{capsuleCenter, box.getReferential().unitI, box.getExtI() * 2.f, sphereRadius};
}

Capsule MovingSphereOrientedBox::getLeftForwardVeronoiCapsule(const OrientedBox& box, float sphereRadius)
{
    Vec3 capsuleCenter = box.getReferential().origin - box.getReferential().unitI * box.getExtI() + box.getReferential().unitK * box.getExtK();
    return Capsule{capsuleCenter, box.getReferential().unitJ, box.getExtJ() * 2.f, sphereRadius};
}

Capsule MovingSphereOrientedBox::getLeftBackwardVeronoiCapsule(const OrientedBox& box, float sphereRadius)
{
    Vec3 capsuleCenter = box.getReferential().origin - box.getReferential().unitI * box.getExtI() - box.getReferential().unitK * box.getExtK();
    return Capsule{capsuleCenter, box.getReferential().unitJ, box.getExtJ() * 2.f, sphereRadius};
}

Capsule MovingSphereOrientedBox::getRightForwardVeronoiCapsule(const OrientedBox& box, float sphereRadius)
{
    Vec3 capsuleCenter = box.getReferential().origin + box.getReferential().unitI * box.getExtI() + box.getReferential().unitK * box.getExtK();
    return Capsule{capsuleCenter, box.getReferential().unitJ, box.getExtJ() * 2.f, sphereRadius};
}

Capsule MovingSphereOrientedBox::getRightBackwardVeronoiCapsule(const OrientedBox& box, float sphereRadius)
{
    Vec3 capsuleCenter = box.getReferential().origin + box.getReferential().unitI * box.getExtI() - box.getReferential().unitK * box.getExtK();
    return Capsule{capsuleCenter, box.getReferential().unitJ, box.getExtJ() * 2.f, sphereRadius};
}

Quad MovingSphereOrientedBox::getTopVeronoiFace (const OrientedBox& box, float sphereRadius)
{
    Referential quadRef = box.getReferential();
    quadRef.origin += box.getReferential().unitJ * (sphereRadius + box.getExtJ());
    quadRef.unitJ = box.getReferential().unitK;
    quadRef.unitK = box.getReferential().unitJ;
    return Quad{quadRef, box.getExtI(), box.getExtK()};
}

Quad MovingSphereOrientedBox::getBottomVeronoiFace(const OrientedBox& box, float sphereRadius)
{
    Referential quadRef = box.getReferential();
    quadRef.origin -= box.getReferential().unitJ * (sphereRadius + box.getExtJ());
    quadRef.unitJ = -box.getReferential().unitK;
    quadRef.unitK = -box.getReferential().unitJ;
    return Quad{quadRef, box.getExtI(), box.getExtK()};
}

Quad MovingSphereOrientedBox::getRightVeronoiFace(const OrientedBox& box, float sphereRadius)
{
    Referential quadRef = box.getReferential();
    quadRef.origin += box.getReferential().unitI * (sphereRadius + box.getExtI());

    quadRef.unitI = -box.getReferential().unitK;
    quadRef.unitK = box.getReferential().unitI;

    return Quad{quadRef, box.getExtK(), box.getExtJ()};
}

Quad MovingSphereOrientedBox::getLeftVeronoiFace(const OrientedBox& box, float sphereRadius)
{
    Referential quadRef = box.getReferential();
    quadRef.origin -= box.getReferential().unitI * (sphereRadius + box.getExtI());

    quadRef.unitI = box.getReferential().unitK;
    quadRef.unitK = -box.getReferential().unitI;

    return Quad{quadRef, box.getExtK(), box.getExtJ()};
}

Quad MovingSphereOrientedBox::getFowardVeronoiFace(const OrientedBox& box, float sphereRadius)
{
    Referential quadRef = box.getReferential();
    quadRef.origin += box.getReferential().unitK * (sphereRadius + box.getExtK());
    return Quad{quadRef, box.getExtI(), box.getExtJ()};
}

Quad MovingSphereOrientedBox::getBackwardVeronoiFace(const OrientedBox& box, float sphereRadius)
{
    Referential quadRef = box.getReferential();
    quadRef.origin -= box.getReferential().unitK * box.getExtK();
    quadRef.unitI = -box.getReferential().unitI;
    quadRef.unitK = -box.getReferential().unitK;
    return Quad{quadRef, box.getExtI(), box.getExtJ()};
}