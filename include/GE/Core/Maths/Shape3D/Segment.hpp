//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 09 h 53

#ifndef _SEGMENT_H
#define _SEGMENT_H

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/Shape3D/Plane.hpp"
#include "GE/Core/Maths/Shape3D/Line.hpp"

#include <limits>

namespace Engine::Core::Maths::Shape3D
{
    class Segment
    {
        public:

        #pragma region constructor/destructor

        Segment ()					                = default;
        Segment (const Segment& other)			    = default;
        Segment (Segment&& other)				    = default;
        virtual ~Segment ()				            = default;
        Segment& operator=(Segment const& other)	= default;
        Segment& operator=(Segment && other)		= default;

        explicit Segment (const Vec3& pt1, const Vec3& pt2)
            :   pt1_    {pt1},
                pt2_    {pt2}
        {}

        #pragma endregion //!constructor/destructor

        #pragma region methods

        Plane getLeftPlane () const noexcept
        {
            return Plane(pt1_, (pt1_ - pt2_).normalize());
        }

        Plane getRightPlane () const noexcept
        {
            return Plane(pt2_, (pt2_ - pt1_).normalize());
        }

        float getDistanceWithPoint(Vec3 pt) const noexcept
        {
            Vec3 AB = pt2_ - pt1_;

            if (Plane::getSignedDistanceToPlane(getLeftPlane(), pt) > std::numeric_limits<float>::epsilon())
            {
                if(Plane::getSignedDistanceToPlane(getRightPlane(), pt) > std::numeric_limits<float>::epsilon())
                {
                    Vec3 AC = pt - pt1_;
                    //The point in between the 2 plan. So The result is the rejection of the vectoo AC on AB
                    return (AC - ((Vec3::dot(AC, AB) / Vec3::dot(AB, AB)) * AB)).length();
                }   
                else
                {
                    //The point is behind the plane of point B. The result is the length of the vector BC
                    return (pt - pt2_).length();
                }
            }
            else
            {
                //The point is behind the plane of point A. The result is the length of the vector AC
                return (pt - pt1_).length();
            }
        }

        #pragma endregion //!methods

        #pragma region accessor

        const Vec3& getPt1() const noexcept { return pt1_; }
        const Vec3& getPt2() const noexcept { return pt2_; }

        #pragma endregion //!accessor

        #pragma region mutator

        void setPt1(const Vec3& newPt) noexcept { pt1_ = newPt; }
        void setPt2(const Vec3& newPt) noexcept { pt2_ = newPt; }

        #pragma endregion //!mutator

        #pragma region convertor

        explicit operator Line () const noexcept //use static_cast<Line>(seg) to convert seg to Line
        {
            return Line(pt1_, (pt2_ - pt1_).normalize());
        }

        #pragma endregion //!convertor

        protected:

        #pragma region attribut

        Vec3      pt1_, pt2_;

        #pragma endregion //!attribut

        private:

    };
} /*namespace Engine::Core::Maths::Shape3D*/

#endif //_SEGMENT_H