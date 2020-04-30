//Project : 3d
//Editing by Six Jonathan
//Date : 23/01/2020 - 13 h 22

#ifndef _PLANE_H
#define _PLANE_H

#include "GE/Core/Maths/vec.hpp"

namespace Engine::Core::Maths
{
    class Plane
    {
        public:

            #pragma region constructor/destructor

            Plane (float distance, const Vec3& normal);
            Plane (const Vec3& p1, const Vec3& p2, const Vec3& p3);
            Plane (const Vec3& p1, const Vec3& normal);

            Plane ()					    = delete;
            Plane (const Plane& other)		= default;
            Plane (Plane&& other)			= default;
            virtual ~Plane ()				= default;
            Plane& operator=(Plane const&)	= default;
            Plane& operator=(Plane &&)		= default;

            #pragma endregion //!constructor/destructor

            #pragma region method
            #pragma endregion //!methods

            #pragma region static methods

            /**
             * @brief Get the Plans Intersection between 3 plan. Throw if 
             * 
             * @param plane1 
             * @param plane2 
             * @param plane3 
             * @param point : return pointor with nullptr if intersection not found 
             * @return void 
             */
            static
            void getPlansIntersection (const Plane& plane1, const Plane& plane2, const Plane& plane3, Vec3** point);

            
            /**
             * @brief Get the Signed Distance To Plane object with a point
             * 
             * @param plane 
             * @param point 
             * @return float 
             */
            static
            float getSignedDistanceToPlane(const Plane& plane, const Vec3& point);

            /**
             * @brief Get the Closest Point To Plane object
             * 
             * @param plane 
             * @param point 
             * @return Vec3 
             */
            static
            Vec3 getClosestPointToPlane(const Plane& plane, const Vec3& point);

            /**
             * @brief Get the Distance From Point To Plan object
             * 
             * @param plane 
             * @param point 
             * @return * float 
             */
            static
            float getDistanceFromPointToPlan(const Plane& plane, const Vec3& point);

            /**
             * @brief return true if point is on plane
             * 
             * @param plane 
             * @param point 
             * @return true 
             * @return false 
             */
            static
            bool isOnPlane (const Plane& plane, const Vec3& point);

            /**
             * @brief return true if a point is between two plane inclueding. [plane, plane]
             * 
             * @param plan1 
             * @param plan2 
             * @param pt 
             * @return float 
             */
            static
            bool vecIsBetweenPlanes (const Plane& plane1, const Plane& plane2, const Vec3& pt);

            #pragma endregion //!static methods

            #pragma region accessor
            #pragma endregion //!accessor

            #pragma region mutator
            #pragma endregion //!mutator

            #pragma region operator
            #pragma endregion //!operator

            #pragma region convertor
            #pragma endregion //!convertor

        protected:

            #pragma region attribut

            Vec3 normal_;       //unit vector
            float distance_;    //Distance with origin

            #pragma endregion //!attribut

            #pragma region static attribut
            #pragma endregion //! static attribut

            #pragma region methods
            #pragma endregion //!methods

        private:

    };
}

#endif // _PLANE_H