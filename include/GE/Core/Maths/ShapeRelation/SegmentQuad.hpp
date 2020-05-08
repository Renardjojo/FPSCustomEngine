//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 16 h 05

#ifndef _SEGMENT_QUAD_H
#define _SEGMENT_QUAD_H

#include "GE/Core/Maths/ShapeRelation/Intersection.hpp"
#include "GE/Core/Maths/Shape3D/Segment.hpp"
#include "GE/Core/Maths/Shape3D/Quad.hpp"

namespace Engine::Core::Maths::ShapeRelation
{
    class SegmentQuad
    {
        public:

        #pragma region constructor/destructor

        SegmentQuad ()					                    = delete;
        SegmentQuad (const SegmentQuad& other)			    = delete;
        SegmentQuad (SegmentQuad&& other)				    = delete;
        virtual ~SegmentQuad ()				                = delete;
        SegmentQuad& operator=(SegmentQuad const& other)    = delete;
        SegmentQuad& operator=(SegmentQuad && other)		= delete;

        #pragma endregion //!constructor/destructor

        #pragma region static methods

        static bool isSegmentQuadCollided(const Shape3D::Segment& seg, const Shape3D::Quad& quad, Intersection& intersection);

        #pragma endregion //!static methods

        private :

        #pragma region static methods

        /*using the Cohen–Sutherland algorithm to detect the emplacement of the point*/
        static int detectPointEmplacement (const Vec2& point, const Vec2& min, const Vec2& max);

        static bool detectCollisionOn2DQuad(const Shape3D::Quad& quad, const Shape3D::Segment& seg, Intersection& intersection);

        #pragma endregion //!static methods

        #pragma region static attribut

        //OutCode for detect the position of a point with the capsule
        static const int INSIDE  = 0;   // 0000
        static const int LEFT    = 1;   // 0001
        static const int RIGHT   = 2;   // 0010
        static const int BOTTOM  = 4;   // 0100
        static const int TOP     = 8;   // 1000

        static const int CENTRAL_COLMN_MASK  = 2;  // 0011
        static const int CENTRAL_LINE_MASK   = 12; // 1100

        #pragma endregion //!static attribut
    };

} /*namespace Engine::Core::Maths::ShapeRelation*/

#endif //_SEGMENT_QUAD_H