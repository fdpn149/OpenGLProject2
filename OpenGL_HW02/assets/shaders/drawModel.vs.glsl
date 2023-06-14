#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 normal;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
	gl_Position = projMat * viewMat * modelMat * vec4(aPos, 1.0);

	normal = aNormal;
}