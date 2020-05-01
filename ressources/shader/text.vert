layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 uvs;

out vec3 ourColor;
uniform mat4 matrix;

void main()
{
    gl_Position = matrix * vec4(aPos, 1.0);
    ourColor = uvs;
}
