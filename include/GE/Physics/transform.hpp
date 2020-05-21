//Project : Engine
//Editing by Six Jonathan
//Date : 10/01/2020 - 13 h 59

#ifndef _GE_TRANSFORM_H
#define _GE_TRANSFORM_H

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/mat.hpp"
#include "GE/Core/Debug/log.hpp"

namespace Engine::Physics
{
    typedef struct S_TransformCreateArg
    {
        Engine::Core::Maths::Vec3   position = Engine::Core::Maths::Vec3::zero, 
                                    rotation = Engine::Core::Maths::Vec3::zero, 
                                    scale    = Engine::Core::Maths::Vec3::one;
    } TransformCreateArg;

    //class inline only
    class Transform
    {
        public:
    
            #pragma region constructor/destructor

            Transform () = default;

            Transform ( const Engine::Core::Maths::Vec3& position, 
                        const Engine::Core::Maths::Vec3& rotation, 
                        const Engine::Core::Maths::Vec3& scale)
                : position_ (position),
                  rotation_ (rotation),
                  scale_    (scale),
                  modelMat_ (),
                  isDirty_  (true)
            {
                update();
            }

            Transform (const TransformCreateArg& arg)
                : Transform (arg.position, arg.rotation, arg.scale)
            {}

            Transform (const Transform& other) = default;
            ~Transform () = default;
    
            #pragma endregion //!constructor/destructor
    
            #pragma region methods

            /**
             * @brief update Mesh matrix 
             * 
             */
            virtual void update () noexcept
            {
                if (!isDirty_)
                    return;

                modelMat_ = Engine::Core::Maths::Mat4::createTRSMatrix(scale_, rotation_, position_);
                isDirty_ = false;
            }

            /**
             * @brief update transform if it depend to parent Mesh view (use in inherance in scene graph)
             * 
             * @param parentMeshMatrix : Mesh view matrix of parent
             */
            virtual void update (const Engine::Core::Maths::Mat4& parentMeshMatrix) noexcept
            {
                modelMat_ = parentMeshMatrix * Engine::Core::Maths::Mat4::createTRSMatrix(scale_, rotation_, position_);
                isDirty_ = false;  
            }

            #pragma endregion //!methods
    
            #pragma region accessor

            virtual const Engine::Core::Maths::Mat4& getModelMatrix () const noexcept { return modelMat_; }

            /**
             * @brief return true if object was modify and must by update
             * 
             * @return true 
             * @return false 
             */
            virtual bool isDirty() const noexcept { return isDirty_; }

            virtual const Engine::Core::Maths::Vec3& getPosition() const noexcept   { return position_; }
            virtual const Engine::Core::Maths::Vec3& getRotation() const noexcept   { return rotation_; }
            virtual const Engine::Core::Maths::Vec3& getScale() const noexcept      { return scale_; }

            virtual const Engine::Core::Maths::Vec3 getVecForward() const noexcept  { return modelMat_.getVectorForward(); }
            virtual const Engine::Core::Maths::Vec3 getVecRight() const noexcept    { return modelMat_.getVectorRight(); }
            virtual const Engine::Core::Maths::Vec3 getVecUp() const noexcept       { return modelMat_.getVectorUp(); }

            virtual Engine::Core::Maths::Vec3 getGlobalPosition() const noexcept   { return {modelMat_[3][0], modelMat_[3][1], modelMat_[3][2]}; }
            virtual Engine::Core::Maths::Vec3 getGlobalScale() const noexcept      { return {modelMat_[0][0], modelMat_[1][1], modelMat_[2][2]}; }

            #pragma endregion //!accessor
    
            #pragma region mutator

            virtual void rotate (float angleRad, Engine::Core::Maths::Vec3 rotAxis)
            {
                rotAxis.normalize();
                rotation_ += {rotAxis.x * angleRad, rotAxis.y * angleRad, rotAxis.z * angleRad};
                isDirty_ = true;
            }

            virtual void translate (Engine::Core::Maths::Vec3 translation)
            {
                position_ += translation;
                isDirty_ = true;
            }

            virtual void scale (Engine::Core::Maths::Vec3 scale)
            {
                scale_ += scale;
                isDirty_ = true;
            }

            virtual void setRotation (float angleRad, Engine::Core::Maths::Vec3 rotAxis)
            {
                rotAxis.normalize();
                rotation_ = {rotAxis.x * angleRad, rotAxis.y * angleRad, rotAxis.z * angleRad};
                isDirty_ = true;
            }

            virtual void setRotation (Engine::Core::Maths::Vec3 rotation)
            {
                rotation_ = rotation;
                isDirty_ = true;
            }

            virtual void setTranslation (Engine::Core::Maths::Vec3 translation)
            {
                position_ = translation;
                isDirty_ = true;
            }

            virtual void setScale (Engine::Core::Maths::Vec3 scale)
            {
                scale_ = scale;
                isDirty_ = true;
            }

            /**
             * @brief Set the Model Matrix object. This function can be use to copy the TRS matrix of entity to an other. This function don't update position, scaole and rotation info
             * 
             * @param modelMat 
             */
            virtual void setModelMatrix (const Engine::Core::Maths::Mat4& modelMat)
            {
                modelMat_ = modelMat;
                isDirty_ = true;
            }

            /**
             * @brief Set the Dirty object and force it to be update
             * 
             */
            void setDirty() { isDirty_ = true;}

            #pragma endregion //!mutator
    
            #pragma region operator
            #pragma endregion //!operator
    
            #pragma region convertor
            #pragma endregion //!convertor
    
        protected:
    
            #pragma region attribut

            Engine::Core::Maths::Vec3 position_;
            Engine::Core::Maths::Vec3 rotation_;
            Engine::Core::Maths::Vec3 scale_;
            Engine::Core::Maths::Mat4 modelMat_; //Local coordinates are the coordinates of object relative to its local origin; they're the coordinates object begins in.
            bool                      isDirty_; //Flag on if transform class must be update

            #pragma endregion //!attribut
    
            #pragma region static attribut
            #pragma endregion //! static attribut
    
            #pragma region methods
            #pragma endregion //!methods
    
        private:
    
    };
}

#endif // _GE_TRANSFORM_H
