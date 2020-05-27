layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoord = aTexCoord;
   	vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww; 
}  