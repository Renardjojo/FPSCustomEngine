//Project : Engine
//Editing by Six Jonathan
//Date : 09/01/2020 - 13 h 05

#ifndef _GE_Mesh_H
#define _GE_Mesh_H

#include <vector>
#include <string>
#include <memory>

#include "GE/Core/Maths/mat.hpp"
#include "glad/glad.h"
#include "GE/Ressources/type.hpp"
#include "GE/Ressources/shader_type.hpp"
#include "GE/Ressources/material.hpp"


namespace Engine::Ressources
{
    typedef struct S_MeshConstructorArg
    {
        std::string                             objName;
        std::vector<Engine::Core::Maths::Vec3>  vBuffer;
        std::vector<Engine::Core::Maths::Vec2>  vtBuffer;
        std::vector<Engine::Core::Maths::Vec3>  vnBuffer;
        std::vector<std::vector<Engine::Ressources::Indice>> iBuffer;

    } MeshConstructorArg;

    class Mesh
    {
        public :

        enum class Axis
        {
            X, 
            Y,
            Z
        };

        public:
    
            #pragma region constructor/destructor
    
            Mesh (const char* objPath, bool loadInGPU = true);
            Mesh (MeshConstructorArg meshArg, bool loadInGPU = true);
            Mesh (const Attrib& attrib, const std::vector<Shape>& shape, bool loadInGPU = true);

            Mesh (const Mesh& other)  = delete;
            Mesh (Mesh&& other)  = default;
            ~Mesh ();
    
            #pragma endregion //!constructor/destructor
    
            #pragma region methods

            /**
             * @brief Load Mesh from CPU to GPU. This operation can be slow but use more faster the Mesh.
             * 
             */
            void loadInGPU () noexcept;
            void unloadFromGPU () noexcept;

            /**
             * @brief return true if Mesh is load in GPU and ready to use
             * 
             * @return true 
             * @return false 
             */
            bool isLoadInGPU () const noexcept { return isLoadInGPU_; }

            /**
             * @brief Drawn Mesh
             * 
             */
            void draw () const noexcept;

            #pragma endregion //!methods
    
            #pragma region static methods

            /**
             * @brief Create a plae object of radius 1 and return it mesh. Plane is centered on the origin
             * 
             * @param textureRepetition     : if > 1 : repetition else split
             * @param indexTexture          : index of texture if split
             * @return MeshConstructorArg 
             */
            static 
            MeshConstructorArg createPlane	(float textureRepetition = 1.f, unsigned int indexTextureX = 0, unsigned int indexTextureY = 0, Axis towardAxis = Axis::Y);

            /**
             * @brief Create a Cube object of radius 1 and return it mesh. Cube is centered on the origin
             * 
             * @return MeshConstructorArg 
             */
            static 
            MeshConstructorArg createCube	(float textureRepetition = 1.f);

            /**
             * @brief Create a Sphere object of radius 1 and return it mesh. Sphere is centered on the origin
             * 
             * @param latitudeCount     : number of vertex in latitude
             * @param longitudeCount    : number of vertex in longitude
             * @return MeshConstructorArg 
             */
            static
            MeshConstructorArg createSphere(int latitudeCount, int longitudeCount);

            /**
             * @brief Create a Cylindre object
             * 
             * @param prescision 
             * @return MeshConstructorArg 
             */
            static 
            MeshConstructorArg createCylindre(unsigned int prescision); //TODO:: add uv

            #pragma endregion //!static methods
    
            #pragma region accessor

            /**
             * @brief return the id of Mesh load in GPU.
             * 
             * @return const GLuint* : nullptr if Mesh is not load in GPU
             */
            const GLuint*  getVAOId () noexcept { return &indexVAO_; }

            /**
             * @brief Get the Indices object
             * 
             * @return const std::vector<std::vector<Engine::Core::Indice>>& 
             */
            const std::vector<std::vector<Engine::Ressources::Indice>>&  getIndices () const noexcept { return iBuffer_; }

            /**
             * @brief Get the Id Materials object
             * 
             * @return const std::vector<std::string>& 
             */
            const std::vector<std::string>& getIdMaterials () const noexcept { return idMaterial_; }

            #pragma endregion //!accessor
    
            #pragma region mutator
            #pragma endregion //!mutator
    
            #pragma region operator
            #pragma endregion //!operator
    
            #pragma region convertor
            #pragma endregion //!convertor
    
        protected:
    
            #pragma region attribut

            GLuint                                                  indexVAO_;
            bool                                                    isLoadInGPU_;

            std::string                                             objName_;
            std::vector<Engine::Core::Maths::Vec3>                  vBuffer_;
            std::vector<Engine::Core::Maths::Vec2>                  vtBuffer_;
            std::vector<Engine::Core::Maths::Vec3>                  vnBuffer_;
            std::vector<std::vector<Engine::Ressources::Indice>>    iBuffer_;
            std::vector<std::string>                                idMaterial_;

            #pragma endregion //!attribut
        
        private:
    
        //this function generate the normal of object. Must be use if obj file don't contain normal.
        void generateNormalAndLoadIndice(const std::vector<Shape>& shapes) noexcept;
    };
} //namespace Engine::Ressources

#endif // _GE_Mesh_H