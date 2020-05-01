layout (location = 0) in vec3 aPos;

out vec3 ourColor;
uniform mat4 matrix;
uniform vec3 color;

void main()
{
    gl_Position = matrix * vec4(aPos, 1.0);
    ourColor = color * vec3(1,1,1);
}
