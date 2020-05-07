//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 16 h 05

#ifndef _MOVING_SPHERE_ORIENTED_BOX_H
#define _MOVING_SPHERE_ORIENTED_BOX_H

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/ShapeRelation/Intersection.hpp"
#include "GE/Core/Maths/Shape3D/Segment.hpp"
#include "GE/Core/Maths/Shape3D/Capsule.hpp"
#include "GE/Core/Maths/Shape3D/Quad.hpp"
#include "GE/Core/Maths/ShapeRelation/SegmentOrientedBox.hpp"
#include "GE/Core/Maths/ShapeRelation/SegmentCapsule.hpp"
#include "GE/Core/Maths/Referential.hpp"

namespace Engine::Core::Maths::ShapeRelation
{
    //OutCode for detect the position of a point with the capsule
    #define BOTTOM_LEFT_BACKWARD  0    // 0000
    #define TOP_LEFT_BACKWARD     1    // 0001
    #define BOTTOM_RIGHT_BACKWARD 2    // 0010
    #define TOP_RIGHT_BACKWARD    3    // 0011
    #define BOTTOM_LEFT_FORWARD   4    // 0100
    #define TOP_LEFT_FORWARD      5    // 0101
    #define BOTTOM_RIGHT_FORWARD  6    // 0110
    #define TOP_RIGHT_FORWARD     7    // 0111
    #define INSIDE                8    // 1000

    #define ON_TOP_MASK           0    // 0001
    #define ON_RIGHT_MASK         1    // 0010
    #define ON_FORWARD_MASK       2    // 0100

    class MovingSphereOrientedBox
    {
        public:

        #pragma region constructor/destructor

        MovingSphereOrientedBox ()					                                = delete;
        MovingSphereOrientedBox (const MovingSphereOrientedBox& other)			    = delete;
        MovingSphereOrientedBox (MovingSphereOrientedBox&& other)				    = delete;
        virtual ~MovingSphereOrientedBox ()				                            = delete;
        MovingSphereOrientedBox& operator=(MovingSphereOrientedBox const& other)    = delete;
        MovingSphereOrientedBox& operator=(MovingSphereOrientedBox && other)		= delete;

        #pragma endregion //!constructor/destructor

        #pragma region static methods

        /*Get the first collision point between moving sphere and static box*/
        static bool IsMovingSphereOrientedBoxCollided(const Shape3D::Sphere& sphere, const Shape3D::OrientedBox& box, const Vec3& sphereVelocity, Intersection& intersection)
        {
            Shape3D::OrientedBox minkowskiSumOBB = getMinkowskiSumOBB(box, sphere.getRadius());
            Shape3D::Segment spherePt1ToPt2 {sphere.getCenter(), sphere.getCenter() + sphereVelocity};


            /*Step 1, check if the segment collid with the Minkowski sum box*/
            if (!SegmentOrientedBox::IsSegmentOrientedBoxCollided(spherePt1ToPt2, minkowskiSumOBB, intersection))
            {
                intersection.SetNotIntersection();
                return false;
            }

            /*Step 2, check if intersection points are on the veronoi face*/
            applyVeronoiRegionCorrection(box, intersection, spherePt1ToPt2, sphere.getRadius());

            return intersection.intersectionType != EIntersectionType::NoIntersection;
        }

        #pragma endregion //!static methods

        private :

        #pragma region static methods

        static Shape3D::OrientedBox getMinkowskiSumOBB (const Shape3D::OrientedBox& box, float sphereRadius)
        {
            return Shape3D::OrientedBox{box.getReferential(), box.getExtI() + sphereRadius, box.getExtJ() + sphereRadius, box.getExtK() + sphereRadius};
        }

        static void applyVeronoiRegionCorrection(const Shape3D::OrientedBox& box, Intersection& intersection, const Shape3D::Segment& seg, float sphereRadius)
        {
            if (intersection.intersectionType == EIntersectionType::OneIntersectiont)
            {
                int topVeronoiOutCode = GetTopVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(intersection.intersection1);
                int rightVeronoiOutCode = GetRightVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(intersection.intersection1);
                int forwardVeronoiOutCode = GetFowardVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(intersection.intersection1);

                if (topVeronoiOutCode == QUAD_OUTCODE_INSIDE || rightVeronoiOutCode == QUAD_OUTCODE_INSIDE || forwardVeronoiOutCode == QUAD_OUTCODE_INSIDE)
                {
                    return;
                }

                /*Define if AB or BA in function of the position of the first segment point. If This point is inside do BA else AB*/
                int topVeronoiOutCodeSeg1 = GetTopVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(seg.getPt1());
                int rightVeronoiOutCodeSeg1 = GetRightVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(seg.getPt1());
                int forwardVeronoiOutCodeSeg1 = GetFowardVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(seg.getPt1());

                if (topVeronoiOutCodeSeg1 == QUAD_OUTCODE_INSIDE && rightVeronoiOutCodeSeg1 == QUAD_OUTCODE_INSIDE && forwardVeronoiOutCodeSeg1 == QUAD_OUTCODE_INSIDE)
                {
                    applyVeronoiRegionCorrectionWithOutCode(box, intersection, Shape3D::Segment{seg.getPt2(), seg.getPt1()}, sphereRadius, topVeronoiOutCode, rightVeronoiOutCode, forwardVeronoiOutCode, true);
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

                int topVeronoiOutCodePt1 = GetTopVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(intersection.intersection1);
                int rightVeronoiOutCodePt1 = GetRightVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(intersection.intersection1);
                int forwardVeronoiOutCodePt1 = GetFowardVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(intersection.intersection1);

                int topVeronoiOutCodePt2 = GetTopVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(intersection.intersection2);
                int rightVeronoiOutCodePt2 = GetRightVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(intersection.intersection2);
                int forwardVeronoiOutCodePt2 = GetBackwardVeronoiFace(box, sphereRadius).isPointInsideQuadZoneOutCode(intersection.intersection2);

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
        }

        static void applyVeronoiRegionCorrectionWithOutCode(const Shape3D::OrientedBox& box, Intersection& intersection, const Shape3D::Segment& seg, float sphereRadius, int topOutCode, int rightOutCode, int forwardOutCode, bool checkFirstIntersection)
        {
            if (topOutCode == QUAD_OUTCODE_INSIDE || rightOutCode == QUAD_OUTCODE_INSIDE || forwardOutCode == QUAD_OUTCODE_INSIDE)
            {
                return;
            }

            if ((forwardOutCode & QUAD_OUTCODE_TOP) == QUAD_OUTCODE_TOP)
            {
                if ((topOutCode & QUAD_OUTCODE_TOP) == QUAD_OUTCODE_TOP)
                {
                    applyCapsuleCorrection(seg, GetTopForwardVeronoiCapsule(box, sphereRadius), intersection, checkFirstIntersection);
                }
                else if ((topOutCode & QUAD_OUTCODE_BOTTOM) == QUAD_OUTCODE_BOTTOM)
                {
                    applyCapsuleCorrection(seg, GetTopBackwardVeronoiCapsule(box, sphereRadius), intersection, checkFirstIntersection);
                }
                else if ((topOutCode & QUAD_OUTCODE_LEFT) == QUAD_OUTCODE_LEFT)
                {
                    applyCapsuleCorrection(seg, GetTopLeftVeronoiCapsule(box, sphereRadius), intersection, checkFirstIntersection);
                }
                else
                {
                    applyCapsuleCorrection(seg, GetTopRightVeronoiCapsule(box, sphereRadius), intersection, checkFirstIntersection);
                }
            }
            else if ((forwardOutCode& QUAD_OUTCODE_BOTTOM) == QUAD_OUTCODE_BOTTOM)
            {
                if ((topOutCode & QUAD_OUTCODE_TOP) == QUAD_OUTCODE_TOP)
                {
                    applyCapsuleCorrection(seg, GetBottomForwardVeronoiCapsule(box, sphereRadius), intersection, checkFirstIntersection);
                }
                else if ((topOutCode & QUAD_OUTCODE_BOTTOM) == QUAD_OUTCODE_BOTTOM)
                {
                    applyCapsuleCorrection(seg, GetBottomBackwardVeronoiCapsule(box, sphereRadius), intersection, checkFirstIntersection);
                }
                else if ((topOutCode & QUAD_OUTCODE_LEFT) == QUAD_OUTCODE_LEFT)
                {
                    applyCapsuleCorrection(seg, GetBottomLeftVeronoiCapsule(box, sphereRadius), intersection, checkFirstIntersection);
                }
                else
                {
                    applyCapsuleCorrection(seg, GetBottomRightVeronoiCapsule(box, sphereRadius), intersection, checkFirstIntersection);
                }
            }
            else /*On the center*/
            {
                if ((topOutCode & (QUAD_OUTCODE_TOP | QUAD_OUTCODE_LEFT)) == QUAD_OUTCODE_TOP + QUAD_OUTCODE_LEFT)
                {
                    applyCapsuleCorrection(seg, GetLeftForwardVeronoiCapsule(box, sphereRadius), intersection, checkFirstIntersection);
                }
                else if ((topOutCode & (QUAD_OUTCODE_TOP | QUAD_OUTCODE_RIGHT)) == QUAD_OUTCODE_TOP + QUAD_OUTCODE_RIGHT)
                {
                    applyCapsuleCorrection(seg, GetRightForwardVeronoiCapsule(box, sphereRadius), intersection, checkFirstIntersection);
                }
                else if ((topOutCode & (QUAD_OUTCODE_BOTTOM | QUAD_OUTCODE_LEFT)) == QUAD_OUTCODE_BOTTOM + QUAD_OUTCODE_LEFT)
                {
                    applyCapsuleCorrection(seg, GetLeftBackwardVeronoiCapsule(box, sphereRadius), intersection, checkFirstIntersection);
                }
                else
                {
                    applyCapsuleCorrection(seg, GetRightBackwardVeronoiCapsule(box, sphereRadius), intersection, checkFirstIntersection);
                }
            }
        }

        static void applyCapsuleCorrection(const Shape3D::Segment& seg, const Shape3D::Capsule& _capsule, Intersection& intersection, bool checkFirstIntersection)
        {
            Intersection shapeIntersection;
            if (SegmentCapsule::IsSegmentCapsuleCollided(seg, _capsule, shapeIntersection))
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
            }
            else
            {
                if (checkFirstIntersection)
                {
                    intersection.RemoveFirstIntersection();
                }
                else
                {
                    intersection.RemoveSecondIntersection();
                }
            }
        }


        static Shape3D::Capsule GetTopLeftVeronoiCapsule(const Shape3D::OrientedBox& box, float sphereRadius)
        {
            Vec3 capsuleCenter = box.getReferential().origin - box.getReferential().unitI * box.getExtI() + box.getReferential().unitJ * box.getExtJ();
            return Shape3D::Capsule{capsuleCenter, box.getReferential().unitK, box.getExtK() * 2.f, sphereRadius};
        }

        static Shape3D::Capsule GetTopRightVeronoiCapsule(const Shape3D::OrientedBox& box, float sphereRadius)
        {
            Vec3 capsuleCenter = box.getReferential().origin + box.getReferential().unitI * box.getExtI() + box.getReferential().unitJ * box.getExtJ();
            return Shape3D::Capsule{capsuleCenter, box.getReferential().unitK, box.getExtK()* 2.f, sphereRadius};
        }

        static Shape3D::Capsule GetTopForwardVeronoiCapsule(const Shape3D::OrientedBox& box, float sphereRadius)
        {
            Vec3 capsuleCenter = box.getReferential().origin + box.getReferential().unitK * box.getExtK() + box.getReferential().unitJ * box.getExtJ();
            return Shape3D::Capsule{capsuleCenter, box.getReferential().unitI, box.getExtI() * 2.f, sphereRadius};
        }

        static Shape3D::Capsule GetTopBackwardVeronoiCapsule(const Shape3D::OrientedBox& box, float sphereRadius)
        {
            Vec3 capsuleCenter = box.getReferential().origin - box.getReferential().unitK * box.getExtK() + box.getReferential().unitJ * box.getExtJ();
            return Shape3D::Capsule{capsuleCenter, box.getReferential().unitI, box.getExtI() * 2.f, sphereRadius};
        }

        static Shape3D::Capsule GetBottomLeftVeronoiCapsule(const Shape3D::OrientedBox& box, float sphereRadius)
        {
            Vec3 capsuleCenter = box.getReferential().origin - box.getReferential().unitI * box.getExtI() - box.getReferential().unitJ * box.getExtJ();
            return Shape3D::Capsule{capsuleCenter, box.getReferential().unitK, box.getExtK() * 2.f, sphereRadius};
        }

        static Shape3D::Capsule GetBottomRightVeronoiCapsule(const Shape3D::OrientedBox& box, float sphereRadius)
        {
            Vec3 capsuleCenter = box.getReferential().origin + box.getReferential().unitI * box.getExtI() - box.getReferential().unitJ * box.getExtJ();
            return Shape3D::Capsule{capsuleCenter, box.getReferential().unitK, box.getExtK() * 2.f, sphereRadius};
        }

        static Shape3D::Capsule GetBottomForwardVeronoiCapsule(const Shape3D::OrientedBox& box, float sphereRadius)
        {
            Vec3 capsuleCenter = box.getReferential().origin + box.getReferential().unitK * box.getExtK() - box.getReferential().unitJ * box.getExtJ();
            return Shape3D::Capsule{capsuleCenter, box.getReferential().unitI, box.getExtI() * 2.f, sphereRadius};
        }

        static Shape3D::Capsule GetBottomBackwardVeronoiCapsule(const Shape3D::OrientedBox& box, float sphereRadius)
        {
            Vec3 capsuleCenter = box.getReferential().origin - box.getReferential().unitK * box.getExtK() - box.getReferential().unitJ * box.getExtJ();
            return Shape3D::Capsule{capsuleCenter, box.getReferential().unitI, box.getExtI() * 2.f, sphereRadius};
        }

        static Shape3D::Capsule GetLeftForwardVeronoiCapsule(const Shape3D::OrientedBox& box, float sphereRadius)
        {
            Vec3 capsuleCenter = box.getReferential().origin - box.getReferential().unitI * box.getExtI() + box.getReferential().unitK * box.getExtK();
            return Shape3D::Capsule{capsuleCenter, box.getReferential().unitJ, box.getExtJ() * 2.f, sphereRadius};
        }

        static Shape3D::Capsule GetLeftBackwardVeronoiCapsule(const Shape3D::OrientedBox& box, float sphereRadius)
        {
            Vec3 capsuleCenter = box.getReferential().origin - box.getReferential().unitI * box.getExtI() - box.getReferential().unitK * box.getExtK();
            return Shape3D::Capsule{capsuleCenter, box.getReferential().unitJ, box.getExtJ() * 2.f, sphereRadius};
        }

        static Shape3D::Capsule GetRightForwardVeronoiCapsule(const Shape3D::OrientedBox& box, float sphereRadius)
        {
            Vec3 capsuleCenter = box.getReferential().origin + box.getReferential().unitI * box.getExtI() + box.getReferential().unitK * box.getExtK();
            return Shape3D::Capsule{capsuleCenter, box.getReferential().unitJ, box.getExtJ() * 2.f, sphereRadius};
        }

        static Shape3D::Capsule GetRightBackwardVeronoiCapsule(const Shape3D::OrientedBox& box, float sphereRadius)
        {
            Vec3 capsuleCenter = box.getReferential().origin + box.getReferential().unitI * box.getExtI() - box.getReferential().unitK * box.getExtK();
            return Shape3D::Capsule{capsuleCenter, box.getReferential().unitJ, box.getExtJ() * 2.f, sphereRadius};
        }

        static Shape3D::Quad GetTopVeronoiFace (const Shape3D::OrientedBox& box, float sphereRadius)
        {
            Referential quadRef = box.getReferential();
            quadRef.origin += box.getReferential().unitJ * (sphereRadius + box.getExtJ());
            quadRef.unitJ = box.getReferential().unitK;
            quadRef.unitK = box.getReferential().unitJ;
            return Shape3D::Quad{quadRef, box.getExtI(), box.getExtK()};
        }

        static Shape3D::Quad GetBottomVeronoiFace(const Shape3D::OrientedBox& box, float sphereRadius)
        {
            Referential quadRef = box.getReferential();
            quadRef.origin -= box.getReferential().unitJ * (sphereRadius + box.getExtJ());
            quadRef.unitJ = -box.getReferential().unitK;
            quadRef.unitK = -box.getReferential().unitJ;
            return Shape3D::Quad{quadRef, box.getExtI(), box.getExtK()};
        }

        static Shape3D::Quad GetRightVeronoiFace(const Shape3D::OrientedBox& box, float sphereRadius)
        {
            Referential quadRef = box.getReferential();
            quadRef.origin += box.getReferential().unitI * (sphereRadius + box.getExtI());

            quadRef.unitI = -box.getReferential().unitK;
            quadRef.unitK = box.getReferential().unitI;

            return Shape3D::Quad{quadRef, box.getExtK(), box.getExtJ()};
        }

        static Shape3D::Quad GetLeftVeronoiFace(const Shape3D::OrientedBox& box, float sphereRadius)
        {
            Referential quadRef = box.getReferential();
            quadRef.origin -= box.getReferential().unitI * (sphereRadius + box.getExtI());

            quadRef.unitI = box.getReferential().unitK;
            quadRef.unitK = -box.getReferential().unitI;

            return Shape3D::Quad{quadRef, box.getExtK(), box.getExtJ()};
        }

        static Shape3D::Quad GetFowardVeronoiFace(const Shape3D::OrientedBox& box, float sphereRadius)
        {
            Referential quadRef = box.getReferential();
            quadRef.origin += box.getReferential().unitK * (sphereRadius + box.getExtK());
            return Shape3D::Quad{quadRef, box.getExtI(), box.getExtJ()};
        }

        static Shape3D::Quad GetBackwardVeronoiFace(const Shape3D::OrientedBox& box, float sphereRadius)
        {
            Referential quadRef = box.getReferential();
            quadRef.origin -= box.getReferential().unitK * box.getExtK();
            quadRef.unitI = -box.getReferential().unitI;
            quadRef.unitK = -box.getReferential().unitK;
            return Shape3D::Quad{quadRef, box.getExtI(), box.getExtJ()};
        }


        #pragma endregion //!static methods
    };

} /*namespace Engine::Core::Maths::ShapeRelation*/

#endif //_MOVING_SPHERE_ORIENTED_BOX_H