//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 14 h 13

#ifndef _CYLINDER_H
#define _CYLINDER_H

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/Shape3D/Segment.hpp"
#include "GE/Core/Maths/Shape3D/Plane.hpp"
#include "GE/Core/Maths/Shape3D/InfiniteCylinder.hpp"

namespace Engine::Core::Maths::Shape3D
{
    class Cylinder
    {
        public:
    
        #pragma region constructor/destructor
    
        Cylinder ()                           = default;
        Cylinder(const Cylinder& other)       = default;
        Cylinder( Cylinder&& other)           = default;
        virtual ~Cylinder()                   = default;
        Cylinder& operator=(Cylinder const&)  = default;
        Cylinder& operator=(Cylinder &&)      = default;

        explicit Cylinder (const Vec3& center, const Vec3& normal, float height, float radius)
            :   segment_  {Vec3(center - normal * (height / 2.f)), Vec3(center + normal * (height / 2.f))},
                radius_   {radius}
        {}

        explicit Cylinder (const Segment& segment, float radius)
        :   segment_    {segment},
            radius_     {radius}
        {}

        explicit Cylinder (const Vec3& pt1, const Vec3& pt2, float radius)
        :   segment_    (pt1, pt2),
            radius_  {radius}
        {}
    
        #pragma endregion //!constructor/destructor
    
        #pragma region methods

        InfiniteCylinder getInfiniteCylinder() const noexcept
        {
            return InfiniteCylinder(static_cast<Line>(segment_), radius_);
        }

        Plane LeftPlane () const noexcept
        {
            return Plane(segment_.getPt1(), (segment_.getPt1() - segment_.getPt2()).getNormalize());
        }

        Plane RightPlane () const noexcept
        {
            return Plane(segment_.getPt2(), (segment_.getPt2() - segment_.getPt1()).getNormalize());
        }

        Vec3 getCenter() const noexcept
        {
            return segment_.getCenter();
        }

        #pragma endregion //!methods
    
        #pragma region accessor

        const Segment&  getSegment  () const noexcept   { return segment_;}
        Segment&        getSegment  () noexcept         { return segment_;}
        const float&    getRadius   () const noexcept   { return radius_;}

        #pragma endregion //!accessor
    
        #pragma region mutator

        void setSegment     (const Segment& newSegment)noexcept { segment_ = newSegment;}
        void setRadius      (const float& newRadius)   noexcept { radius_ = newRadius;}

        #pragma endregion //!mutator
    
        protected:
    
        #pragma region attribut

        Segment    segment_;
        float   radius_;

        #pragma endregion //!attribut
    
        private:
    
    };

} /*namespace Engine::Core::Maths::Shape3D*/

#endif //_CYLINDER_H