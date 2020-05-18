//Project : Engine
//Editing by Six Jonathan
//Date : 13/01/2020 - 18 h 19

#ifndef _GE_CAMERA_H
#define _GE_CAMERA_H

#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/mat.hpp"

namespace Engine::LowRenderer
{
    enum class E_ProjectionType
    {
        PERSPECTIVE,
        ORTHOGRAPHIC
    };

   typedef struct S_ProjectionInfo
   {
        std::string name;
        E_ProjectionType type;

        float aspect;
        float near, far;
        float left, right, top, bottom;
        float fovY, fovX;

    } ProjectionInfo;

    struct CameraPerspectiveCreateArg
    {
        const Engine::Core::Maths::Vec3& position; 
        const Engine::Core::Maths::Vec3& rotation;
        float aspect; 
        float near; 
        float far; 
        float fovY = 70.f;
        const char* name = "Camera";     
    };

    struct CameraOrthographicCreateArg
    {
        const Engine::Core::Maths::Vec3& position; 
        const Engine::Core::Maths::Vec3& rotation;
        float left; 
        float right; 
        float bottom; 
        float top; 
        float nearVal; 
        float farVal;
        const char* name = "Camera";     
    };

    class Camera
        : public Engine::Ressources::GameObject
    {
        public:

            #pragma region constructor/destructor

            Camera ()                           = default;

            /**
             * @brief Construct a new perspective camera object
             * 
             * @param position 
             * @param rotation 
             * @param aspect    : width/height
             * @param near      : must be > 0 
             * @param far 
             * @param fovY      : by default to 70 (human FovY)
             * @param name 
             */
            Camera (const CameraPerspectiveCreateArg& arg);

            /**
             * @brief Construct a new orthographic camera object
             * 
             * @param position 
             * @param rotation 
             * @param left 
             * @param right 
             * @param bottom 
             * @param top 
             * @param nearVal 
             * @param farVal 
             * @param name 
             */
            Camera (const CameraOrthographicCreateArg& arg);

            Camera (const Camera& other)		= default;
            Camera (Camera&& other)				= default;
            virtual ~Camera ()				    = default;

            #pragma endregion //!constructor/destructor
            
            #pragma region methods

            /**
             * @brief Use this camera to show the next obj drawed
             * 
             */
            void use () noexcept
            {
                camToUse = this;
            }

            /**
             * @brief update Mesh matrix 
             * 
             */
            void update () noexcept override;

            /**
             * @brief update transform if it depend to parent Mesh view (use in inherance in scene graph)
             * 
             * @param parentMeshMatrix : Mesh view matrix of parent
             */
            void update (const Engine::Core::Maths::Mat4& parentMeshMatrix) noexcept override;        

            /**
             * @brief construct view matrix with look at arg. 
             * 
             * @param eye    : Specifies the position of the eye point
             * @param center : Specifies the position of the reference point
             * @param up     : Specifies the direction of the up vector
             */
            void lookAt (const Engine::Core::Maths::Vec3& eye, const Engine::Core::Maths::Vec3& center, const Engine::Core::Maths::Vec3& up) noexcept;

            /**
             * @brief Set the Fov Y object
             * 
             * @param fovY 
             */
            void setFovY(const float fovY) noexcept; 

            #pragma endregion //!methods

            #pragma region static methods         

            static Camera* getCamUse() noexcept { return camToUse; }

            #pragma endregion //!static methods


            #pragma region accessor
        
            /**
             * @brief Get the Projection Info in specifie index
             * 
             * @return const ProjectionInfo& 
             */
            const ProjectionInfo& getProjectionInfo() const noexcept { return projInfo_; }

            /**
             * @brief Get the view * projection matrix
             * 
             * @return const Engine::Core::Maths::Mat4& 
             */
            const Engine::Core::Maths::Mat4& getViewProjection() const noexcept { return viewProjectionMatrix_; }

            /**
             * @brief Get the view matrix
             * 
             * @return const Engine::Core::Maths::Mat4& 
             */
            const Engine::Core::Maths::Mat4& getView() const noexcept { return viewMatrix_; }

            /**
             * @brief Get the projection matrix
             * 
             * @return const Engine::Core::Maths::Mat4& 
             */
            const Engine::Core::Maths::Mat4& getProjection() const noexcept { return projection_; }

            #pragma endregion //!accessor

            #pragma region mutator
            #pragma endregion //!mutator

            #pragma region operator

            Camera& operator=(const Camera& other) = default;

            #pragma endregion //!operator

            #pragma region convertor
            #pragma endregion //!convertor

        protected:

            #pragma region attribut

            ProjectionInfo              projInfo_;
            Engine::Core::Maths::Mat4   projection_;

            Engine::Core::Maths::Mat4   viewMatrix_;
            Engine::Core::Maths::Mat4   viewProjectionMatrix_;

            #pragma endregion //!attribut

            #pragma region static attribut

            static Camera* camToUse; //pointor to be in nullptr by default

            #pragma endregion //! static attribut

        private:

    };

} //namespace Engine::LowRenderer

#endif // _GE_CAMERA_H