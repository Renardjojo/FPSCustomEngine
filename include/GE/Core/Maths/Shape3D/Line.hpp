//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 11 h 57

#ifndef _LINE_H
#define _LINE_H

#include "GE/Core/Maths/vec.hpp"

namespace Engine::Core::Maths::Shape3D
{
    class Line
    {
        public : 

        #pragma region constructor/destructor

        Line ()                             = default;
        Line(const Line& other)             = default;
        Line(Line&& other)                  = default;
        ~Line()                             = default;
        Line& operator=(Line const&)        = default;
        Line& operator=(Line &&)            = default; 

        explicit Line (const Vec3& origin, const Vec3& normal)
            :   origin_     {origin},
                normal_ {normal}
        {}

        #pragma endregion //!constructor/destructor

        #pragma region accessor

        const Vec3& getOrigin() const noexcept    { return origin_; }
        const Vec3& getNormal() const noexcept    { return normal_; }

        #pragma endregion //!accessor

        #pragma region mutator

        void setOrigin(const Vec3& newOrigin) noexcept { origin_ = newOrigin; }
        void setNormal(const Vec3& newNormal) noexcept { normal_ = newNormal; }

        #pragma endregion //!mutator

        protected :

        Vec3    origin_, 
                normal_;
    };
} /*namespace Engine::Core::Maths::Shape3D*/
#endif //_LINE_H


