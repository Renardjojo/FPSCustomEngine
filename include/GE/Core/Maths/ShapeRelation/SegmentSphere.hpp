//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 16 h 13

#ifndef _SEGMENT_SPHERE_H
#define _SEGMENT_SPHERE_H

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/ShapeRelation/Intersection.hpp"
#include "GE/Core/Maths/Shape3D/Segment.hpp"
#include "GE/Core/Maths/Shape3D/Sphere.hpp"

namespace Engine::Core::Maths::ShapeRelation
{
    class SegmentSphere
    {
        public:

        #pragma region constructor/destructor

        SegmentSphere ()					                    = delete;
        SegmentSphere (const SegmentSphere& other)			    = delete;
        SegmentSphere (SegmentSphere&& other)				    = delete;
        virtual ~SegmentSphere ()				                = delete;
        SegmentSphere& operator=(SegmentSphere const& other)    = delete;
        SegmentSphere& operator=(SegmentSphere && other)		= delete;

        #pragma endregion //!constructor/destructor

        #pragma region static methods

        static bool IsSegmentSphereCollided(const Shape3D::Segment& seg, const Shape3D::Sphere& sphere, Intersection& intersection)
        {
            Vec3 AOmega         = sphere.getCenter() - seg.getPt1();
            float AOmegaLength  = AOmega.length();
            Vec3 AB             = seg.getPt2() - seg.getPt1();
            float ABLength      = AB.length();

            /*AOmega² - R + 2AB. OmegaA * t + AB² * t² = 0*/
            float a = ABLength * ABLength;
            float b = 2.f * Vec3::dot(AB, -AOmega);
            float c = AOmegaLength * AOmegaLength - sphere.getRadius() * sphere.getRadius();

            /*We comput the discriminent*/
            if (Intersection::ComputeDiscriminentAndSolveEquation(a, b, c, seg.getPt1(), seg.getPt2(), intersection))
            {
                if (intersection.intersectionType == EIntersectionType::TwoIntersectiont)
                {
                    intersection.normalI2 = (intersection.intersection2 - sphere.getCenter()).normalize();
                }
                intersection.normalI1 = (intersection.intersection1 - sphere.getCenter()).normalize();

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

#endif //_SEGMENT_SPHERE_H