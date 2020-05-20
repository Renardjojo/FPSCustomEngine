//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 13 h 46

#ifndef _ORIENTED_BOX_H
#define _ORIENTED_BOX_H

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/mat.hpp"
#include "GE/Core/Maths/Referential.hpp"
#include "GE/Core/Maths/Shape3D/AABB.hpp"

namespace Engine::Core::Maths::Shape3D
{
    class OrientedBox
    {
        public :

        #pragma region constructor/destructor

        OrientedBox ()                              = default;
        OrientedBox(const OrientedBox& other)       = default;
        OrientedBox(OrientedBox&& other)            = default;
        virtual ~OrientedBox()                      = default;
        OrientedBox& operator=(OrientedBox const&)  = default;
        OrientedBox& operator=(OrientedBox &&)      = default;

        explicit OrientedBox (float rightLenght, float upLenght, float forwardLenght, const Vec3& center = Vec3::zero, const Vec3& rotation = Vec3::zero)
            :   referential_    {center},
                iI_             {rightLenght}, 
                iJ_             {upLenght}, 
                iK_             {forwardLenght}
        {
            Mat3 rotationMatrix = Mat3::createFixedAngleEulerRotationMatrix(rotation);
            referential_.unitI = rotationMatrix.getVectorRight();
            referential_.unitJ = rotationMatrix.getVectorUp();
            referential_.unitK = rotationMatrix.getVectorForward();
        }

        explicit OrientedBox(const Referential& referential, float rightLenght, float upLenght, float forwardLenght)
            :   referential_    {referential},
                iI_             {rightLenght}, 
                iJ_             {upLenght}, 
                iK_             {forwardLenght}
        {}

        #pragma endregion //!constructor/destructor

        #pragma region methods

        Shape3D::AABB getAABB() const noexcept
        {
            Vec3 vecIi = referential_.unitI * iI_;
            Vec3 vecIj = referential_.unitJ * iJ_;
            Vec3 vecIk = referential_.unitK * iK_;
            float AABBiI = std::abs(Vec3::dot(Vec3::right, vecIi)) + std::abs(Vec3::dot(Vec3::right, vecIj)) + std::abs(Vec3::dot(Vec3::right, vecIk));
            float AABBiJ = std::abs(Vec3::dot(Vec3::up, vecIi)) + std::abs(Vec3::dot(Vec3::up, vecIj)) + std::abs(Vec3::dot(Vec3::up, vecIk));
            float AABBiK = std::abs(Vec3::dot(Vec3::forward, vecIi)) + std::abs(Vec3::dot(Vec3::forward, vecIj)) + std::abs(Vec3::dot(Vec3::forward, vecIk));
            return Shape3D::AABB{referential_.origin, AABBiI, AABBiJ, AABBiK};
        }

        Vec3 ptForwardTopLeft     () const noexcept { return referential_.origin - (referential_.unitI * iI_) + (referential_.unitJ * iJ_) + (referential_.unitK * iK_); }
        Vec3 ptForwardTopRight    () const noexcept { return referential_.origin + (referential_.unitI * iI_) + (referential_.unitJ * iJ_) + (referential_.unitK * iK_); }
        Vec3 ptForwardBottomLeft  () const noexcept { return referential_.origin - (referential_.unitI * iI_) - (referential_.unitJ * iJ_) + (referential_.unitK * iK_); }
        Vec3 ptForwardBottomRight () const noexcept { return referential_.origin + (referential_.unitI * iI_) - (referential_.unitJ * iJ_) + (referential_.unitK * iK_); }
        Vec3 ptBackTopLeft        () const noexcept { return referential_.origin - (referential_.unitI * iI_) + (referential_.unitJ * iJ_) - (referential_.unitK * iK_); }
        Vec3 ptBackTopRight       () const noexcept { return referential_.origin + (referential_.unitI * iI_) + (referential_.unitJ * iJ_) - (referential_.unitK * iK_); }
        Vec3 ptBackBottomLeft     () const noexcept { return referential_.origin - (referential_.unitI * iI_) - (referential_.unitJ * iJ_) - (referential_.unitK * iK_); }
        Vec3 ptBackBottomRight    () const noexcept { return referential_.origin + (referential_.unitI * iI_) - (referential_.unitJ * iJ_) - (referential_.unitK * iK_); }

        #pragma endregion //!methods

        #pragma region accessor

        virtual Referential  getReferential()    const noexcept  { return referential_; }
        Referential&         getReferential()          noexcept  { return referential_; }
        virtual float        getExtI()           const noexcept  { return iI_; }
        virtual float        getExtJ()           const noexcept  { return iJ_; }
        virtual float        getExtK()           const noexcept  { return iK_; }

        #pragma endregion //!accessor

        #pragma region mutator

        void setReferential (const Referential& newReferential)    noexcept   {  referential_ = newReferential; }
        void setExtI        (const float& newExtI)                 noexcept   {  iI_ = newExtI; }
        void setExtJ        (const float& newExtJ)                 noexcept   {  iJ_ = newExtJ; }
        void setExtK        (const float& newExtK)                 noexcept   {  iK_ = newExtK; }

        #pragma endregion //!mutator

        protected:

        #pragma region attribut

        Referential referential_;
        float       iI_ {0.f}, iJ_ {0.f}, iK_ {0.f}; 

        #pragma endregion //!attribut

        private:

    };
} /*namespace Engine::Core::Maths::Shape3D*/

#endif //_ORIENTED_BOX_H
