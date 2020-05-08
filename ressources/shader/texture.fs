in vec3 ourUVS;
out vec4 FragColor;
uniform sampler2D myTextureSampler;

void main()
{

    vec4 texColor = texture(myTextureSampler, vec2(ourUVS.x, ourUVS.y));
    
    if(texColor.a < 0.1)
        discard;

    FragColor = texColor;
    
}