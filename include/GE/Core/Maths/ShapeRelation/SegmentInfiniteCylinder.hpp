//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 17 h 20

#ifndef _SEGMENT_INFINITE_CYLINDER_H
#define _SEGMENT_INFINITE_CYLINDER_H

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/ShapeRelation/Intersection.hpp"
#include "GE/Core/Maths/Shape3D/Segment.hpp"
#include "GE/Core/Maths/Shape3D/InfiniteCylinder.hpp"

namespace Engine::Core::Maths::ShapeRelation
{
    class SegmentInfiniteCylinder
    {
        public:

        #pragma region constructor/destructor

        SegmentInfiniteCylinder ()					                                = delete;
        SegmentInfiniteCylinder (const SegmentInfiniteCylinder& other)			    = delete;
        SegmentInfiniteCylinder (SegmentInfiniteCylinder&& other)				    = delete;
        virtual ~SegmentInfiniteCylinder ()				                            = delete;
        SegmentInfiniteCylinder& operator=(SegmentInfiniteCylinder const& other)    = delete;
        SegmentInfiniteCylinder& operator=(SegmentInfiniteCylinder && other)		= delete;

        #pragma endregion //!constructor/destructor

        #pragma region static methods

        static bool IsSegmentInfiniteCylinderCollided(const Shape3D::Segment& seg, const Shape3D::InfiniteCylinder& infCylinder, Intersection& intersection)
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
            if (Intersection::ComputeDiscriminentAndSolveEquation(a, b, c, seg.getPt1(), seg.getPt2(), intersection))
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

        #pragma endregion //!static methods

        private :

        #pragma region static methods

        #pragma endregion //!static methods
    };

} /*namespace Engine::Core::Maths::ShapeRelation*/

#endif //_SEGMENT_INFINITE_CYLINDER_H