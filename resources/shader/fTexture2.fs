out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D ourTexture;

void main()
{
    vec4 texColor = texture(ourTexture, TexCoord);
    
    if(texColor.a < 0.1)
        discard;

    FragColor = texColor * vec4(computLight(lightBuffer, numberLightUse, Normal, FragPos), 1.0);
}