out vec4 FragColor;
in vec3 Normal;

void main()
{
    FragColor = Color * vec4(computLight(lightBuffer, numberLightUse, Normal, FragPos), 1.0);
}