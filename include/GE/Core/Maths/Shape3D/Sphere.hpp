//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 12 h 33

#ifndef _SPHERE_H
#define _SPHERE_H

#include "GE/Core/Maths/vec.hpp"

namespace Engine::Core::Maths::Shape3D
{
    class Sphere
    {
        public :

        #pragma region constructor/destructor

        Sphere ()                           = default;
        Sphere(const Sphere& other)         = default;
        Sphere(Sphere&& other)              = default;
        virtual ~Sphere()                   = default;
        Sphere& operator=(Sphere const&)    = default;
        Sphere& operator=(Sphere &&)        = default; 

        explicit Sphere (float radius, const Vec3& localCenter = Vec3::zero)
            :   center_        {localCenter},
                radius_             {radius}
        {}
    
        #pragma endregion //!constructor/destructor
    
        #pragma region accessor

        virtual Vec3     getCenter() const noexcept { return center_;}
        virtual float    getRadius() const noexcept { return radius_;}

        #pragma endregion //!accessor
    
        #pragma region mutator

        void    setCenter(const Vec3&  newCenter) noexcept { center_ = newCenter;}
        void    setRadius(const float& newRadius) noexcept { radius_ = newRadius;}

        #pragma endregion //!mutator
    
        protected:
    
        #pragma region attribut

        Vec3            center_ {Vec3::zero};
        float           radius_ {0.f};

        #pragma endregion //!attribut
    
        private:
    };
} /*namespace Engine::Core::Maths::Shape3D*/

#endif //_SPHERE_H


