//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 09 h 04

#ifndef _AABB_H
#define _AABB_H

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/MathTools.hpp"

namespace Engine::Core::Maths::Shape3D
{
    class AABB
    {
        public:

        #pragma region constructor/destructor

        AABB ()					                = default;
        AABB (const AABB& other)			    = default;
        AABB (AABB&& other)				        = default;
        virtual ~AABB ()				        = default;
        AABB& operator=(AABB const& other)		= default;
        AABB& operator=(AABB && other)			= default;

        AABB(const Vec3& center, float iI, float iJ, float iK)
            :   center_  {center},
                iI_     {iI},
                iJ_     {iJ},
                iK_     {iK}
        {}

        #pragma endregion //!constructor/destructor

        #pragma region methods

        bool isInside(Vec3 pt) const noexcept
        {
            Vec3 localPt = pt - center_;
            const float espilon = 0.0001f; /*The standard float espsilone his enought in this case. The float value has chang to much*/

            return  isBetween(localPt.x, -iI_ - espilon, iI_ + espilon) &&
                    isBetween(localPt.y, -iJ_ - espilon, iJ_ + espilon) &&
                    isBetween(localPt.z, -iK_ - espilon, iK_ + espilon);
        }

        #pragma endregion //!methods

        #pragma region accessor

        virtual Vec3    getCenter() const noexcept  { return center_; }
        const float&    getExtI()   const noexcept  { return iI_; }
        const float&    getExtJ()   const noexcept  { return iJ_; }
        const float&    getExtK()   const noexcept  { return iK_; }

        #pragma endregion //!accessor

        #pragma region mutator

        void setCenter (const Vec3& center) noexcept   {  center_ = center; }
        void setExtI   (const float& i)     noexcept   {  iI_ = i; }
        void setExtJ   (const float& j)     noexcept   {  iJ_ = j; }
        void setExtK   (const float& k)     noexcept   {  iK_ = k; }

        #pragma endregion //!mutator

        protected:

        #pragma region attribut

        Vec3 center_;
        float iI_, iJ_, iK_;

        #pragma endregion //!attribut


        private:

    };

} /*namespace Engine::Core::Maths:Shape3D*/

#endif //_AABB_H