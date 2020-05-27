out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{   
    vec3 result;
    result = vec3(0.0, 0.0, 0.0);

    for(int i = 0; i < numberLightUse; i++)
    {
        if(lightBuffer[i].type == DIRECITONNAL_LIGHT)
        {
            result += computAmbiante(lightBuffer[i].ambient, material.ambient) + computAmbiante(lightBuffer[i].diffuse, material.diffuse);
        }
    }

    FragColor = texture(ourTexture, TexCoord) * vec4(result, 1.0);
}