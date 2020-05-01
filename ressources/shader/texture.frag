in vec3 ourColor;
out vec4 FragColor;
uniform sampler2D myTextureSampler;

void main()
{
    FragColor = texture(myTextureSampler, vec2(ourColor.x, ourColor.y));
}