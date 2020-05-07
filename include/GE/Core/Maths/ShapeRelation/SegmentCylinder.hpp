//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 17 h 30

#ifndef _SEGMENT_SEGMENT_H
#define _SEGMENT_SEGMENT_H

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/ShapeRelation/Intersection.hpp"
#include "GE/Core/Maths/Shape3D/Segment.hpp"
#include "GE/Core/Maths/Shape3D/InfiniteCylinder.hpp"
#include "GE/Core/Maths/Shape3D/Cylinder.hpp"
#include "GE/Core/Maths/Shape3D/Plane.hpp"
#include "GE/Core/Maths/ShapeRelation/SegmentInfiniteCylinder.hpp"
#include "GE/Core/Maths/ShapeRelation/SegmentPlane.hpp"

#include <limits>

namespace Engine::Core::Maths::ShapeRelation
{
    class SegmentCylinder
    {
        public:

        #pragma region constructor/destructor

        SegmentCylinder ()					                        = delete;
        SegmentCylinder (const SegmentCylinder& other)			    = delete;
        SegmentCylinder (SegmentCylinder&& other)				    = delete;
        virtual ~SegmentCylinder ()				                    = delete;
        SegmentCylinder& operator=(SegmentCylinder const& other)    = delete;
        SegmentCylinder& operator=(SegmentCylinder && other)		= delete;

        #pragma endregion //!constructor/destructor

        #pragma region static methods

        static bool IsSegmentCylinderCollided(const Shape3D::Segment& seg, const Shape3D::Cylinder& cylinder, Intersection& intersection)
        {
            Shape3D::InfiniteCylinder infinitCyl = cylinder.getInfiniteCylinder();

            /*If there not have collision this infinit cylindre coaxile with the cylindre return false*/
            if (!SegmentInfiniteCylinder::IsSegmentInfiniteCylinderCollided(seg, infinitCyl, intersection))
            {
                intersection.SetNotIntersection();
                return false;
            }

            /*If the both points of the segment is on the same midle zone of the cylinder return the infiniteCylinder Intersection*/
            Shape3D::Plane leftCylindreFace = cylinder.LeftPlane();
            Shape3D::Plane rightCylindreFace = cylinder.RightPlane();

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
                intersection.SetNotIntersection();
                return false;
            }

            /*There is one intersection with the infinite cylinder*/
            if (intersection.intersectionType == EIntersectionType::OneIntersectiont)
            {
                /*Check if the intersection point is inside the cylinder*/
                if (leftCylindreFace.getSignedDistanceToPlane(intersection.intersection1) > std::numeric_limits<float>::epsilon())
                {
                    Intersection segQuadIntersection;

                    if (SegmentPlane::IsSegmentPlaneCollided(seg, leftCylindreFace, segQuadIntersection))
                    {
                        if (infinitCyl.isPointInside(segQuadIntersection.intersection1))
                        {
                            intersection.intersection1 = segQuadIntersection.intersection1;
                            intersection.normalI1 = pt1InFrontOfLeftFace ? segQuadIntersection.normalI1 : -segQuadIntersection.normalI1;

                            /*Check if there are a second intersection on the other face*/
                            if (SegmentPlane::IsSegmentPlaneCollided(seg, rightCylindreFace, segQuadIntersection))
                            {
                                if (infinitCyl.isPointInside(segQuadIntersection.intersection1))
                                {
                                    intersection.intersection2 = segQuadIntersection.intersection1;
                                    intersection.normalI2 = pt1InFrontOfLeftFace ? -segQuadIntersection.normalI1 : segQuadIntersection.normalI1;
                                    intersection.intersectionType = EIntersectionType::TwoIntersectiont;
                                }
                            }
                            intersection.SortIntersection(seg.getPt1());
                            return true;
                        }
                    }

                    intersection.SetNotIntersection();
                    return false;
                }
                else if (rightCylindreFace.getSignedDistanceToPlane(intersection.intersection1) > std::numeric_limits<float>::epsilon())
                {
                    Intersection segQuadIntersection;

                    if (SegmentPlane::IsSegmentPlaneCollided(seg, rightCylindreFace, segQuadIntersection))
                    {
                        if (infinitCyl.isPointInside(segQuadIntersection.intersection1))
                        {
                            intersection.intersection1 = segQuadIntersection.intersection1;
                            intersection.normalI1 = pt1InFrontOfRightFace ? segQuadIntersection.normalI1 : -segQuadIntersection.normalI1;

                            /*Check if there are a second intersection on the other face*/
                            if (SegmentPlane::IsSegmentPlaneCollided(seg, leftCylindreFace, segQuadIntersection))
                            {
                                if (infinitCyl.isPointInside(segQuadIntersection.intersection1))
                                {
                                    intersection.intersection2 = segQuadIntersection.intersection1;
                                    intersection.normalI2 = pt1InFrontOfRightFace ? -segQuadIntersection.normalI1 : segQuadIntersection.normalI1;
                                    intersection.intersectionType = EIntersectionType::TwoIntersectiont;
                                }
                            }
                            intersection.SortIntersection(seg.getPt1());
                            return true;
                        }
                    }
                    intersection.SetNotIntersection();
                    return false;
                }
                else
                {
                    Intersection segQuadIntersection;
                    if (SegmentPlane::IsSegmentPlaneCollided(seg, leftCylindreFace, segQuadIntersection))
                    {
                        if (infinitCyl.isPointInside(segQuadIntersection.intersection1))
                        {
                            intersection.intersection2 = segQuadIntersection.intersection1;
                            intersection.normalI2 = pt1InFrontOfLeftFace ? segQuadIntersection.normalI1 : -segQuadIntersection.normalI1;
                            intersection.intersectionType = EIntersectionType::TwoIntersectiont;
                            
                            intersection.SortIntersection(seg.getPt1());
                            return true;
                        }
                    }

                    if (SegmentPlane::IsSegmentPlaneCollided(seg, rightCylindreFace, segQuadIntersection))
                    {
                        if (infinitCyl.isPointInside(segQuadIntersection.intersection1))
                        {
                            intersection.intersection2 = segQuadIntersection.intersection1;
                            intersection.normalI2 = pt1InFrontOfRightFace ? segQuadIntersection.normalI1 : -segQuadIntersection.normalI1;
                            intersection.intersectionType = EIntersectionType::TwoIntersectiont;
                            
                            intersection.SortIntersection(seg.getPt1());
                            return true;
                        }
                    }
                    intersection.SortIntersection(seg.getPt1());
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

                    if (SegmentPlane::IsSegmentPlaneCollided(seg, leftCylindreFace, segQuadIntersection))
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
                    if (SegmentPlane::IsSegmentPlaneCollided(seg, rightCylindreFace, segQuadIntersection))
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
                    if (SegmentPlane::IsSegmentPlaneCollided(seg, leftCylindreFace, segQuadIntersection))
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
                    if (SegmentPlane::IsSegmentPlaneCollided(seg, rightCylindreFace, segQuadIntersection))
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
                        intersection.SortIntersection(seg.getPt1());
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
                    intersection.SetOneIntersection(intersection.intersection2);
                    return true;
                }
                else
                {
                    intersection.SetNotIntersection();
                    return false;
                }
            }

            /*The point is on the infiniteCylinder. Not inside the cylinder and note nd the same side. So, Check if there is 1 or 2 collision*/
            if (pt1InFrontOfLeftFace && pt2InFrontOfRightFace)
            {
                Intersection segQuadIntersection;
                SegmentPlane::IsSegmentPlaneCollided(seg, leftCylindreFace, segQuadIntersection);
                intersection.intersection1 = segQuadIntersection.intersection1;
                intersection.normalI1 = pt1InFrontOfLeftFace ? segQuadIntersection.normalI1 : -segQuadIntersection.normalI1;

                SegmentPlane::IsSegmentPlaneCollided(seg, rightCylindreFace, segQuadIntersection);
                intersection.intersection2 = segQuadIntersection.intersection1;
                intersection.normalI2 = pt1InFrontOfLeftFace ? -segQuadIntersection.normalI1 : segQuadIntersection.normalI1;

                intersection.intersectionType = EIntersectionType::TwoIntersectiont;
                return true;
            }
            else if (pt1InFrontOfRightFace && pt2InFrontOfLeftFace)
            {
                Intersection segQuadIntersection;
                SegmentPlane::IsSegmentPlaneCollided(seg, rightCylindreFace, segQuadIntersection);
                intersection.intersection1 = segQuadIntersection.intersection1;
                intersection.normalI1 = pt1InFrontOfRightFace ? segQuadIntersection.normalI1 : -segQuadIntersection.normalI1;

                SegmentPlane::IsSegmentPlaneCollided(seg, leftCylindreFace, segQuadIntersection);
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
                    SegmentPlane::IsSegmentPlaneCollided(seg, rightCylindreFace, segQuadIntersection);
                    intersection.SetOneIntersection(segQuadIntersection.intersection1);
                    intersection.normalI1 = pt1InFrontOfRightFace ? segQuadIntersection.normalI1 : -segQuadIntersection.normalI1;
                }
                else
                {
                    Intersection segQuadIntersection;
                    SegmentPlane::IsSegmentPlaneCollided(seg, leftCylindreFace, segQuadIntersection);
                    intersection.SetOneIntersection(segQuadIntersection.intersection1);
                    intersection.normalI1 = pt1InFrontOfLeftFace ? segQuadIntersection.normalI1 : -segQuadIntersection.normalI1;
                }
                return true;
            }
        }

        #pragma endregion //!static methods

        private :

        #pragma region static methods

        #pragma endregion //!static methods
    };

} /*namespace Engine::Core::Maths::ShapeRelation*/

#endif //_SEGMENT_SEGMENT_H