//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 16 h 05

#ifndef _SEGMENT_AABB_H
#define _SEGMENT_AABB_H

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/ShapeRelation/Intersection.hpp"
#include "GE/Core/Maths/Shape3D/Segment.hpp"
#include "GE/Core/Maths/Shape3D/AABB.hpp"
#include "GE/Core/Maths/MathTools.hpp"

#include <limits>

namespace Engine::Core::Maths::ShapeRelation
{
    class SegmentAABB
    {
        public:

        #pragma region constructor/destructor

        SegmentAABB ()					                    = delete;
        SegmentAABB (const SegmentAABB& other)			    = delete;
        SegmentAABB (SegmentAABB&& other)				    = delete;
        virtual ~SegmentAABB ()				                = delete;
        SegmentAABB& operator=(SegmentAABB const& other)    = delete;
        SegmentAABB& operator=(SegmentAABB && other)		= delete;

        #pragma endregion //!constructor/destructor

        #pragma region static methods

        static bool IsSegmentAABBCollided(Shape3D::Segment seg, Shape3D::AABB AABB, Intersection intersection)
        {
            Vec3 AB = seg.getPt2() - seg.getPt1();
            float tx0, tx1, ty0, ty1, tz0, tz1, tempT;
            tempT = 1.f; // memorise with temporal float the value of T. Compare this value to the new T and compute the nearest point af seg.pt1

            intersection.intersectionType = EIntersectionType::NoIntersection;

            if (!isBetween(AB.x, -std::numeric_limits<float>::epsilon(), std::numeric_limits<float>::epsilon()))
            {
                float invABX = 1.f / AB.x;
                tx0 = (AABB.getExtI() - seg.getPt1().x + AABB.getCenter().x) * invABX;
                tx1 = (-AABB.getExtI() - seg.getPt1().x + AABB.getCenter().x) * invABX;

                addIntersectionWithScalerIfOnAABBAndReturnIfFull(seg, AABB, intersection, tx0, tempT, Vec3::right);

                if (addIntersectionWithScalerIfOnAABBAndReturnIfFull(seg, AABB, intersection, tx1, tempT, Vec3::left))
                {
                    return true;
                }
            }

            if (!isBetween(AB.y, -std::numeric_limits<float>::epsilon(), std::numeric_limits<float>::epsilon()))
            {
                float invABY = 1.f / AB.y;
                ty0 = (AABB.getExtJ() - seg.getPt1().y + AABB.getCenter().y) * invABY;
                ty1 = (-AABB.getExtJ() - seg.getPt1().y + AABB.getCenter().y) * invABY;

                if (addIntersectionWithScalerIfOnAABBAndReturnIfFull(seg, AABB, intersection, ty0, tempT, Vec3::up))
                {
                    return true;
                }

                if (addIntersectionWithScalerIfOnAABBAndReturnIfFull(seg, AABB, intersection, ty1, tempT, Vec3::down))
                {
                    return true;
                }
            }

            if (!isBetween(AB.z, -std::numeric_limits<float>::epsilon(), std::numeric_limits<float>::epsilon()))
            {
                float invABZ = 1.f / AB.z;
                tz0 = (AABB.getExtK() - seg.getPt1().z + AABB.getCenter().z) * invABZ;
                tz1 = (-AABB.getExtK() - seg.getPt1().z + AABB.getCenter().z) * invABZ;

                if (addIntersectionWithScalerIfOnAABBAndReturnIfFull(seg, AABB, intersection, tz0, tempT, Vec3::forward))
                {
                    return true;
                }

                if (addIntersectionWithScalerIfOnAABBAndReturnIfFull(seg, AABB, intersection, tz1, tempT, Vec3::backward))
                {
                    return true;
                }
            }

            return (intersection.intersectionType == EIntersectionType::OneIntersectiont);
        }

        #pragma endregion //!static methods

        private :

        #pragma region static methods

        /*memorise with temporal float the value of T. Compare this value to the new T and compute the nearest point af seg.pt1*/
        static bool addIntersectionWithScalerIfOnAABBAndReturnIfFull(const Shape3D::Segment& seg, const Shape3D::AABB& AABB, Intersection& intersection, float scaler, float& tempT, Vec3 faceNormal)
        {
            if (isBetween(scaler, 0.f, 1.f))
            {
                Vec3 pt = seg.getPt1() + (scaler * (seg.getPt2() - seg.getPt1()));

                if (AABB.isInside(pt))
                {
                    if (intersection.intersectionType != EIntersectionType::OneIntersectiont)
                    {
                        intersection.SetOneIntersection(pt);
                        intersection.normalI1 = faceNormal;

                        tempT = scaler;
                        return false;
                    }
                    else
                    {
                        intersection.SetSecondIntersection(pt);
                        intersection.normalI2 = faceNormal;

                        if (tempT > scaler)
                        {
                            intersection.SwapIntersection();
                        }
                        return true;
                    }
                }
            }
            
            return false;
        }

        #pragma endregion //!static methods
    };

} /*namespace Engine::Core::Maths::ShapeRelation*/

#endif //_SEGMENT_AABB_H