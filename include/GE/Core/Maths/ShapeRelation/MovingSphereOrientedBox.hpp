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
#include "GE/Core/Maths/Shape3D/OrientedBox.hpp"

namespace Engine::Core::Maths::ShapeRelation
{
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

        /*get the first collision point between moving sphere and static box*/
        static bool isMovingSphereOrientedBoxCollided(const Shape3D::Sphere& sphere, const Shape3D::OrientedBox& box, const Vec3& sphereVelocity, Intersection& intersection);

        #pragma endregion //!static methods

        private :

        #pragma region static methods

        static Shape3D::OrientedBox getMinkowskiSumOBB (const Shape3D::OrientedBox& box, float sphereRadius);

        static void applyVeronoiRegionCorrection(const Shape3D::OrientedBox& box, Intersection& intersection, const Shape3D::Segment& seg, float sphereRadius);

        static bool applyVeronoiRegionCorrectionWithOutCode(const Shape3D::OrientedBox& box, Intersection& intersection, const Shape3D::Segment& seg, float sphereRadius, int topOutCode, int rightOutCode, int forwardOutCode, bool checkFirstIntersection);

        static bool applyCapsuleCorrection(const Shape3D::Segment& seg, const Shape3D::Capsule& _capsule, Intersection& intersection, bool checkFirstIntersection);

        /*Sub component of minkowski sum*/
        static Shape3D::Capsule getTopLeftVeronoiCapsule(const Shape3D::OrientedBox& box, float sphereRadius);

        static Shape3D::Capsule getTopRightVeronoiCapsule(const Shape3D::OrientedBox& box, float sphereRadius);

        static Shape3D::Capsule getTopForwardVeronoiCapsule(const Shape3D::OrientedBox& box, float sphereRadius);

        static Shape3D::Capsule getTopBackwardVeronoiCapsule(const Shape3D::OrientedBox& box, float sphereRadius);

        static Shape3D::Capsule getBottomLeftVeronoiCapsule(const Shape3D::OrientedBox& box, float sphereRadius);

        static Shape3D::Capsule getBottomRightVeronoiCapsule(const Shape3D::OrientedBox& box, float sphereRadius);

        static Shape3D::Capsule getBottomForwardVeronoiCapsule(const Shape3D::OrientedBox& box, float sphereRadius);

        static Shape3D::Capsule getBottomBackwardVeronoiCapsule(const Shape3D::OrientedBox& box, float sphereRadius);

        static Shape3D::Capsule getLeftForwardVeronoiCapsule(const Shape3D::OrientedBox& box, float sphereRadius);

        static Shape3D::Capsule getLeftBackwardVeronoiCapsule(const Shape3D::OrientedBox& box, float sphereRadius);

        static Shape3D::Capsule getRightForwardVeronoiCapsule(const Shape3D::OrientedBox& box, float sphereRadius);

        static Shape3D::Capsule getRightBackwardVeronoiCapsule(const Shape3D::OrientedBox& box, float sphereRadius);

        static Shape3D::Quad getTopVeronoiFace (const Shape3D::OrientedBox& box, float sphereRadius);

        static Shape3D::Quad getBottomVeronoiFace(const Shape3D::OrientedBox& box, float sphereRadius);

        static Shape3D::Quad getRightVeronoiFace(const Shape3D::OrientedBox& box, float sphereRadius);

        static Shape3D::Quad getLeftVeronoiFace(const Shape3D::OrientedBox& box, float sphereRadius);

        static Shape3D::Quad getFowardVeronoiFace(const Shape3D::OrientedBox& box, float sphereRadius);

        static Shape3D::Quad getBackwardVeronoiFace(const Shape3D::OrientedBox& box, float sphereRadius);

        #pragma endregion //!static methods

        #pragma region static attribut

        //OutCode for detect the position of a point with the capsule
        static const int BOTTOM_LEFT_BACKWARD  = 0;    // 0000
        static const int TOP_LEFT_BACKWARD     = 1;    // 0001
        static const int BOTTOM_RIGHT_BACKWARD = 2;    // 0010
        static const int TOP_RIGHT_BACKWARD    = 3;    // 0011
        static const int BOTTOM_LEFT_FORWARD   = 4;    // 0100
        static const int TOP_LEFT_FORWARD      = 5;    // 0101
        static const int BOTTOM_RIGHT_FORWARD  = 6;    // 0110
        static const int TOP_RIGHT_FORWARD     = 7;    // 0111
        static const int INSIDE                = 8;    // 1000
    
        static const int ON_TOP_MASK           = 0;    // 0001
        static const int ON_RIGHT_MASK         = 1;    // 0010
        static const int ON_FORWARD_MASK       = 2;    // 0100

        #pragma endregion //!static attribut
    };

} /*namespace Engine::Core::Maths::ShapeRelation*/

#endif //_MOVING_SPHERE_ORIENTED_BOX_H