out vec3 FragPos;

void computFragPos(mat4 Mesh, vec3 aPos)
{
    FragPos = vec3(Mesh * vec4(aPos, 1.0));
}
