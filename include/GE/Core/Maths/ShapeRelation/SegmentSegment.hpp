//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 16 h 05

#ifndef _SEGMENT_SEGMENT_H
#define _SEGMENT_SEGMENT_H

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/ShapeRelation/Intersection.hpp"
#include "GE/Core/Maths/Shape3D/Segment.hpp"

namespace Engine::Core::Maths::ShapeRelation
{
    class SegmentSegment
    {
        public:

        #pragma region constructor/destructor

        SegmentSegment ()					                    = delete;
        SegmentSegment (const SegmentSegment& other)			= delete;
        SegmentSegment (SegmentSegment&& other)				    = delete;
        virtual ~SegmentSegment ()				                = delete;
        SegmentSegment& operator=(SegmentSegment const& other)  = delete;
        SegmentSegment& operator=(SegmentSegment && other)		= delete;

        #pragma endregion //!constructor/destructor

        #pragma region static methods

        static bool IsSegmentSegmentCollided(const Vec3& a1, const Vec3& a2, const Vec3& b1, const Vec3& b2, Intersection& intersection)
        {
            /*using distance between line algorythme. Found reference of this algorythme on : http://paulbourke.net/geometry/pointlineplane/*/

            float sub1 = (d(a2, a1, a2, a1) * d(b2, b1, b2, b1) - d(b2, b1, a2, a1) * d(b2, b1, a2, a1));

            /* if b dot d == 0, it means the lines are parallel so have infinite intersection points*/
            if (sub1 == 0)
                return false;

            float t1 = (d(a1, b1, b2, b1) * d(b2, b1, a2, a1) - d(a1, b1, a2, a1) * d(b2, b1, b2, b1)) / sub1;

            float sub2 = d(b2, b1, b2, b1);

            /* if b dot d == 0, it means the lines are parallel so have infinite intersection points*/
            if (sub2 == 0)
                return false;

            float t2 = (d(a1, b1, b2, b1) + t1 * d(b2, b1, a2, a1)) / sub2;

            if (t1 < 0 || t1 > 1 || t2 < 0 || t2 > 1)
                return false;

            /*OM = OA + t(OB - OA)*/
            if (intersection.intersectionType == EIntersectionType::OneIntersectiont)
            {
                intersection.SetSecondIntersection(a1 + t1 * (a2 - a1));
            }
            else
            {
                intersection.SetOneIntersection(a1 + t1 * (a2 - a1));
            }

            return true;
        }

        #pragma endregion //!static methods

        private :

        #pragma region static methods

        static float d(const Vec3& m, const Vec3& n, const Vec3& o, const Vec3& p)
        {
            return (m.x - n.x) * (o.x - p.x) + (m.y - n.y) * (o.y - p.y) + (m.z - n.z) * (o.z - p.z);
        }

        #pragma endregion //!static methods
    };

} /*namespace Engine::Core::Maths::ShapeRelation*/

#endif //_SEGMENT_SEGMENT_H