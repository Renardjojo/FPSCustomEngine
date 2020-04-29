#define MAX_LIGHT_NUMBER 10

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
    return ambientComponent.w * ambientComponent.xyz * ambientMaterial.xyz;
}

vec3 computDiffuse(vec4 diffuseComponent, vec4 diffuseMaterial, vec3 normal, vec3 fragPos, vec3 lightDir)
{
    float diff = max(dot(normalize(normal), lightDir), 0.0);
    return diffuseComponent.w * diff * diffuseComponent.xyz * diffuseMaterial.xyz;
}

vec3 computSpecular(vec4 specularComponent, vec4 specularMaterial, vec3 normal, vec3 fragPos, vec3 lightDir, vec3 viewPos, float shininess)
{
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normalize(normal));  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    return specularComponent.w * spec * specularComponent.xyz * specularMaterial.xyz;
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
vec3 computLight(Light[MAX_LIGHT_NUMBER] lights, int numberLightUse, vec3 normal, vec3 fragPos, float shininess)
{
    vec3 result;
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
        vec3 spec = computSpecular(lights[i].specular, material.specular, normal, fragPos, lightDir, viewPos, material.shininess);
            
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
