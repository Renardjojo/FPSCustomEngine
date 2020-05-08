//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 16 h 05

#ifndef _SEGMENT_AABB_H
#define _SEGMENT_AABB_H

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/ShapeRelation/Intersection.hpp"
#include "GE/Core/Maths/Shape3D/Segment.hpp"
#include "GE/Core/Maths/Shape3D/AABB.hpp"

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

        static bool isSegmentAABBCollided(Shape3D::Segment seg, Shape3D::AABB AABB, Intersection& intersection);

        #pragma endregion //!static methods

        private :

        #pragma region static methods

        /*memorise with temporal float the value of T. Compare this value to the new T and compute the nearest point af seg.pt1*/
        static bool addIntersectionWithScalerIfOnAABBAndReturnIfFull(const Shape3D::Segment& seg, const Shape3D::AABB& AABB, Intersection& intersection, float scaler, float& tempT, Vec3 faceNormal);

        #pragma endregion //!static methods
    };

} /*namespace Engine::Core::Maths::ShapeRelation*/

#endif //_SEGMENT_AABB_H