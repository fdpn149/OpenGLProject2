#version 410 core
layout (location = 0) in vec3 aPos;

out vec3 texcoord;

uniform mat4 projMat;
uniform mat4 viewMat;

void main()
{
    gl_Position = projMat * viewMat * vec4(aPos, 1.0);
    texcoord = aPos;
}  