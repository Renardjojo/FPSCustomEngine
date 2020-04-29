//Project : Engine
//Editing by Six Jonathan
//Date : 10/01/2020 - 08 h 44

#ifndef _GE_SHADER_H
#define _GE_SHADER_H

#include <string>
#include <vector>
#include "GE/Core/Maths/vec.hpp"
#include "GE/Ressources/shader_type.hpp"

namespace Engine::Ressources
{
    enum class E_ShaderType
    {
        FRAGMENT,
        VERTEX,
        PROGRAM
    };

    //feature mask
    #define LIGHT_BLIN_PHONG            1  // 2^1, bit 1
    #define SKYBOX                      2  // 2^2, bit 2

    //Inspiread about code exemple on learn openGl : https://learnopengl.com/Getting-started/Shaders
    class Shader
    {
    public:

        /**
         * @brief Construct a new Shader object and generates the shader. If feature is added, the feature is added definitely to the shader
         * Use getFeature() to cheack the feature use
         * 
         * @param vertexPath 
         * @param fragmentPath
         * @param featureMask           : LIGHT_BLIN_PHONG | FEATURE_2 | [...]
         */
        Shader(const char* vertexPath, const char* fragmentPath, unsigned char featureMask = 0);
        Shader(const Shader& other) = delete;
        Shader(Shader&& other)      = default;
        ~Shader();

        /**
         * @brief activate the shader
         * 
         */
        void use();

        /**
         * @brief Few function to set value into shader with uniform functions
         * 
         * @param name 
         * @param value 
         */
        void setBool    (const char* name, bool  value) const;
        void setInt     (const char* name, int   value) const;
        void setFloat   (const char* name, float value) const;
        void setVec3   (const char* name, float v1, float v2, float v3) const;
        void setVec4   (const char* name, float v1, float v2, float v3, float v4) const;
        void setMat3   (const char* name, const float* value) const;
        void setMat4   (const char* name, const float* value) const;
        void setpVec3   (const char* name, unsigned int count, const float* pV) const; 
        void setpVec4   (const char* name, unsigned int count, const float* pV) const;
        void setLightBlock      (const std::vector<light>& lightBuffer, const Engine::Core::Maths::Vec3& viewPos);
        void setMaterialBlock   (const MaterialComponent& material);

        unsigned int getIdProgramm() const noexcept { return id_; };

        /**
         * @brief Get the Feature object
         * 
         * @return const std::vector<E_ShaderFeature>& 
         */
        unsigned char getFeature () { return featureMask_; }

    protected : 

        const unsigned char featureMask_; //feature is shader interger into shader like light, blure etc....
        unsigned int lightsUniformBuffer_;
        
        std::string  nameFragment_;
        std::string  nameVertex_;

        unsigned int id_;

    private:

        /**
         * @brief Load shaders files and set code into vertexCode and fragmentCode
         * 
         * @param vertexPath 
         * @param vertexCode 
         * @param fragmentPath 
         * @param fragmentCode 
         */
        void loadFile(const char* vertexPath, std::string& vertexCode,  const char* fragmentPath, std::string& fragmentCode);

        /**
         * @brief Compile 
         * 
         * @param vertexCode 
         * @param fragmentCode
         */
        void compile(std::string& vertexCode, std::string& fragmentCode);

        /**
         * @brief Use log function for checking shader compilation/linking errors
         * 
         * @param shader 
         * @param type 
         */
        void checkCompileErrors(unsigned int shader, E_ShaderType type);

        /**
         * @brief parse the name of image integer in path
         * 
         * @param path 
         * @param shaderName 
         */
        void parseName (const char* path, std::string& shaderName);
    };
}

#endif // _GE_SHADER_H