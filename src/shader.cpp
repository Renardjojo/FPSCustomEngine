#include "GE/Ressources/shader.hpp"
#include "GE/Core/Debug/log.hpp"
#include "glad/glad.h"
#include "GE/Core/Parsers/shaderParser.hpp"

#include <fstream>
#include <sstream>

using namespace Engine::Ressources;
using namespace Engine::Core::Debug;
using namespace Engine::Core::Parsers;
using namespace std;

static const char* versionHeaderStr = R"(#version 330 core

)";

static const char* lightBlinPhongVertexShaderStr = R"(out vec3 FragPos;

void computFragPos(mat4 Mesh, vec3 aPos)
{
    FragPos = vec3(Mesh * vec4(aPos, 1.0));
}

)";

static const char* lightBlinPhongFragmentShaderStr = R"(#define MAX_LIGHT_NUMBER 10

#define LIGHT               0.f
#define POINT_LIGHT         1.f
#define SPOT_LIGHT          2.f
#define DIRECITONNAL_LIGHT  3.f

in vec3 FragPos;
uniform vec3 viewPos;

uniform int numberLightUse;
struct Light {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    vec3 pos;
    float type;

    //Point arg
    float constant;
    float linear;
    float quadratic;

    //Spot arg
    float cutOffExponent;
    vec3 direction;
    float cutOff;
};

struct Material {
    vec4    ambient;
    vec4    diffuse;
    vec4    specular;
    float   shininess;
};

uniform Material material;

layout(std140) uniform lightBlock
{
    Light lightBuffer[MAX_LIGHT_NUMBER];
};

vec3 computAmbiante(vec4 ambientComponent, vec4 ambientMaterial)
{
    return ambientComponent.w * ambientComponent.xyz * ambientMaterial.xyz * ambientMaterial.w;
}

vec3 computDiffuse(vec4 diffuseComponent, vec4 diffuseMaterial, vec3 normal, vec3 fragPos, vec3 lightDir)
{
    float diff = max(dot(normalize(normal), lightDir), 0.0);
    return diffuseComponent.w * diff * diffuseComponent.xyz * diffuseMaterial.xyz * diffuseMaterial.w;
}

vec3 computSpecular(vec4 specularComponent, vec4 specularMaterial, vec3 normal, vec3 fragPos, vec3 lightDir, vec3 viewPos, float shininess)
{
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normalize(normal));  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    return specularComponent.w * spec * specularComponent.xyz * specularMaterial.xyz * specularMaterial.w;
}

float computLightAttenuationWithDistance(vec3 lightPos, vec3 fragPos, float lightConstant, float lightLinear, float lightQuadratic)
{
    float distance    = length(lightPos - fragPos);
    return 1.0 / (lightConstant + lightLinear * distance + lightQuadratic * (distance * distance));
}

float computSpotLightIntensity(vec3 lightDir, vec3 fragPos, vec3 lightDirection, float cutOff, float cutOffExp)
{
    float theta = dot(lightDir, normalize(-lightDirection)); 
    float epsilon = (cutOff - cutOffExp);
    return clamp((theta - cutOffExp) / epsilon, 0.0, 1.0);
}

//FragColor = texture(ourTexture, TexCoord) * vec4(computLight([...]), 1.0);
vec3 computLight(Light[MAX_LIGHT_NUMBER] lights, int numberLightUse, vec3 normal, vec3 fragPos)
{
    vec3 result;
    result = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < numberLightUse; i++)
    {
        vec3 lightDir;

        if(lights[i].type == DIRECITONNAL_LIGHT)
        {
            lightDir = lights[i].pos;
        }
        else
        {
            lightDir = normalize(lights[i].pos - fragPos);
        }

        vec3 amb = computAmbiante(lights[i].ambient, material.ambient);
        vec3 diff = computDiffuse(lights[i].diffuse, material.diffuse, normal, fragPos, lightDir);
        vec3 spec = material.shininess == 0.0 ? vec3(0.0, 0.0, 0.0) : computSpecular(lights[i].specular, material.specular, normal, fragPos, lightDir, viewPos, material.shininess);
            
        if(lights[i].type == DIRECITONNAL_LIGHT)
        {
            result += amb + diff + spec;
        }
        else if(lights[i].type == POINT_LIGHT)
        {
            float attenuation = computLightAttenuationWithDistance(lights[i].pos, fragPos, lights[i].constant, lights[i].linear, lights[i].quadratic);
            result += (amb + diff + spec) * attenuation;
        }
        else if(lights[i].type == SPOT_LIGHT)
        {
            float intensity = computSpotLightIntensity(lightDir, fragPos, lights[i].direction, lights[i].cutOff, lights[i].cutOffExponent);
            float attenuation = computLightAttenuationWithDistance(lights[i].pos, fragPos, lights[i].constant, lights[i].linear, lights[i].quadratic);
            result += (amb + (diff + spec) * intensity) * attenuation;
        }
    }

    return result;
}

)";

Shader::Shader(const char* vertexPath, const char* fragmentPath, unsigned char featureMask)
    :   featureMask_ (featureMask),
        lightsUniformBuffer_ (0)
{
    std::string vertexCode;
    std::string fragmentCode;

    parseName(vertexPath, nameVertex_);
    parseName(fragmentPath, nameFragment_);
    loadFile(vertexPath, vertexCode, fragmentPath, fragmentCode);

    //parse shader : If #include "path" is found, replace by code
    ShaderParser::parse(vertexCode);
    ShaderParser::parse(fragmentCode);

    if ((featureMask_ & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
    {
        vertexCode.insert(0, lightBlinPhongVertexShaderStr);
        fragmentCode.insert(0, lightBlinPhongFragmentShaderStr);
    }

    vertexCode.insert(0, versionHeaderStr);
    fragmentCode.insert(0, versionHeaderStr);

    compile (vertexCode, fragmentCode);

    SLog::log((std::string("Load and compile shaders \"") + nameVertex_.c_str() + "\" shader and \"" + nameFragment_ + "\" shader done").c_str());
}

Shader::~Shader()
{
    glDeleteBuffers(1, &lightsUniformBuffer_);
    glDeleteShader(id_);
}

void Shader::use() 
{
    int idShaderUse;
    glGetIntegerv(GL_CURRENT_PROGRAM, &idShaderUse);

    if ((unsigned int)idShaderUse != id_)
        glUseProgram(id_); 
}

void Shader::setBool(const char* name, bool value) const
{         
    glUniform1i(glGetUniformLocation(id_, name), (int)value);
    functGlCheckAndLogError();
}

void Shader::setInt(const char* name, int value) const
{ 
    glUniform1i(glGetUniformLocation(id_, name), value);
    functGlCheckAndLogError();
}

void Shader::setFloat(const char* name, float value) const
{ 
    glUniform1f(glGetUniformLocation(id_, name), value);
    functGlCheckAndLogError();
}

void Shader::setVec3   (const char* name, float v1, float v2, float v3) const
{
    glUniform3f(glGetUniformLocation(id_, name), v1, v2, v3);
    functGlCheckAndLogError();
}

void Shader::setVec4   (const char* name, float v1, float v2, float v3, float v4) const
{
    glUniform4f(glGetUniformLocation(id_, name), v1, v2, v3, v4);
    functGlCheckAndLogError();
}

void Shader::setMat3   (const char* name, const float* value) const
{
    glUniformMatrix3fv(glGetUniformLocation(id_, name), 1, GL_FALSE, value);
    functGlCheckAndLogError();
}

void Shader::setMat4   (const char* name, const float* value) const
{
    glUniformMatrix4fv(glGetUniformLocation(id_, name), 1, GL_FALSE, value);
    functGlCheckAndLogError();
}

void Shader::setpVec3   (const char* name, unsigned int count, const float* pV) const
{
    glUniform3fv(glGetUniformLocation(id_, name), count, pV);
    functGlCheckAndLogError();
}

void Shader::setpVec4   (const char* name, unsigned int count, const float* pV) const
{
    glUniform4fv(glGetUniformLocation(id_, name), count, pV);
    functGlCheckAndLogError();
}

void Shader::setLightBlock   (const std::vector<light>& lightBuffer, const Engine::Core::Maths::Vec3& viewPos)
{
    if ((featureMask_ & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
    {
        if (lightsUniformBuffer_ == 0)
        {
            glGenBuffers(1, &lightsUniformBuffer_);
            functGlCheckAndLogError();
        }

        glBindBuffer(GL_UNIFORM_BUFFER, lightsUniformBuffer_);
        functGlCheckAndLogError();

        glBufferData(GL_UNIFORM_BUFFER, lightBuffer.size() * sizeof(light), lightBuffer.data(), GL_DYNAMIC_DRAW);
        functGlCheckAndLogError();

        { //try to get uniform block index
            GLuint blockIndex;
            const char* blockName = "lightBlock";
            blockIndex = glGetUniformBlockIndex(id_, blockName);
            functGlCheckAndLogError();

            if (blockIndex == GL_INVALID_INDEX)
            {
                functError((std::string("blockIndex invalid with name : ") + blockName).c_str());
                exit(0);
            }

            glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, lightsUniformBuffer_);
            functGlCheckAndLogError();

        }

        setInt("numberLightUse", lightBuffer.size());
        setVec3("viewPos", viewPos.x, viewPos.y, viewPos.z);
    }
    else
    {
        SLog::logWarning("Shader cannot send light block because it is not a light shader");
    }
}

void Shader::setMaterialBlock   (const MaterialComponent& material)
{
    if ((featureMask_ & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
    {
        setVec4("material.ambient", material.ambient.rgbi.x, material.ambient.rgbi.y, material.ambient.rgbi.z, material.ambient.rgbi.w);
        setVec4("material.diffuse", material.diffuse.rgbi.x,  material.diffuse.rgbi.y,  material.diffuse.rgbi.z,  material.diffuse.rgbi.w);
        setVec4("material.specular", material.specular.rgbi.x, material.specular.rgbi.y, material.specular.rgbi.z, material.specular.rgbi.w);
        setFloat("material.shininess", material.shininess);
    }
    else
    {
        SLog::logWarning("Shader cannot send Material block because it is not a light shader");
    }    
}

void Shader::loadFile(const char* vertexPath, std::string& vertexCode,  const char* fragmentPath, std::string& fragmentCode)
{
    // retrieve the vertex/fragment source code from filePath
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // open files and check error
    vShaderFile.open(vertexPath);
    if (!vShaderFile)
    {
        functError((std::string("Impossible to load shader : ") + vertexPath).c_str());
    }

    fShaderFile.open(fragmentPath);
    if (!vShaderFile)
    {
        functError((std::string("Impossible to load shader : ") + fragmentPath).c_str());
    }

    // read file's buffer contents into streams
    std::stringstream vShaderStream;
    std::stringstream fShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    // close file handlers
    vShaderFile.close();
    fShaderFile.close();

    // convert stream into string
    vertexCode   = vShaderStream.str().c_str();
    fragmentCode = fShaderStream.str().c_str();
}

void Shader::compile(std::string& vertexCode, std::string& fragmentCode)
{
    //compile shaders
    unsigned int vertex, fragment;

    // vertex shader
    const char* vShaderCode = vertexCode.c_str();
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, E_ShaderType::VERTEX);

    // fragment Shader
    const char * fShaderCode = fragmentCode.c_str();
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, E_ShaderType::FRAGMENT);

    // shader Program
    id_ = glCreateProgram();
    glAttachShader(id_, vertex);
    glAttachShader(id_, fragment);
    glLinkProgram(id_);
    checkCompileErrors(id_, E_ShaderType::PROGRAM);

    functGlCheckAndLogError();

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::checkCompileErrors(unsigned int shader, E_ShaderType type)
{
    int success;
    char infoLog[1024];
    if (type != E_ShaderType::PROGRAM)
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);

            if (type == E_ShaderType::VERTEX)
            {
                SLog::logError(std::string("Shader name's \"") + nameVertex_ + "\" compilation error\n" + infoLog);
            }
            else if (type == E_ShaderType::FRAGMENT)
            {
                SLog::logError(std::string("Shader name's \"") + nameFragment_ + "\" compilation error\n" + infoLog);
            }
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);

            if (type == E_ShaderType::VERTEX)
            {
                SLog::logError((std::string("Shader name's \"") + nameVertex_.c_str() + "\" linking error.\n" + infoLog).c_str());
            }
            else if (type == E_ShaderType::FRAGMENT)
            {
                SLog::logError((std::string("Shader name's \"") + nameFragment_.c_str() + "\" linking error.\n" + infoLog).c_str());
            }
        }
    }
}

void Shader::parseName (const char* path, std::string& shaderName)
{
    std::string src(path);

    std::size_t cursStart = src.find_last_of('/');
    if (cursStart == std::string::npos)
        cursStart = 0;
    cursStart += 1; 
    std::size_t cursEnd = src.find_last_of('.');

    shaderName = src.substr(cursStart, cursEnd - cursStart);
}