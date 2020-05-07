//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 16 h 05

#ifndef _SEGMENT_PLANE_H
#define _SEGMENT_PLANE_H

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/ShapeRelation/Intersection.hpp"
#include "GE/Core/Maths/Shape3D/Segment.hpp"
#include "GE/Core/Maths/Shape3D/Plane.hpp"

#include <limits>

namespace Engine::Core::Maths::ShapeRelation
{
    class SegmentPlane
    {
        public:

        #pragma region constructor/destructor

        SegmentPlane ()					                    = delete;
        SegmentPlane (const SegmentPlane& other)			= delete;
        SegmentPlane (SegmentPlane&& other)				    = delete;
        virtual ~SegmentPlane ()				            = delete;
        SegmentPlane& operator=(SegmentPlane const& other)  = delete;
        SegmentPlane& operator=(SegmentPlane && other)		= delete;

        #pragma endregion //!constructor/destructor

        static bool IsSegmentPlaneCollided(const Shape3D::Segment& seg, const Shape3D::Plane& plane, Intersection& intersection)
        {
            Vec3 AB = seg.getPt2() - seg.getPt1();
            float sub = Vec3::dot(AB, plane.getNormal());
            float distanceSegToOrigin = Vec3::dot(seg.getPt1(), plane.getNormal());

            /*if(sub == 0) segmen is orthogonal to the plan*/
            if (std::abs(sub) <= std::numeric_limits<float>::epsilon())
            {
                /*Check if segmen is confused with the plan*/
                /*Compute the distance of the segment with origin and compare with the distance of plan with origin*/

                intersection.SetInifitIntersection();
                return std::abs(distanceSegToOrigin - plane.getDistance()) <= std::numeric_limits<float>::epsilon();
            }

            /*(n . PO( or d ) - n.OA) / n . AB*/
            float t = ((plane.getDistance() - distanceSegToOrigin) / sub);

            /*if t is not between 0 and 1 so not collision else compute OM with OM = OA + t * AB*/
            if (t < 0.f || t > 1.f)
            {
                intersection.SetNotIntersection();
                return false;
            }
            else
            {
                intersection.SetOneIntersection(seg.getPt1() + t * AB);

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

        #pragma region static methods


        #pragma endregion //!static methods

        private :

        #pragma region static methods


        #pragma endregion //!static methods
    };

} /*namespace Engine::Core::Maths::ShapeRelation*/

#endif //_SEGMENT_PLANE_H