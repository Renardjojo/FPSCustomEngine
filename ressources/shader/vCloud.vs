layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out float time;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    time = scaledTimeAcc + aPos.x;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}