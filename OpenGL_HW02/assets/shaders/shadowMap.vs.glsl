#version 410 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightProjMat;
uniform mat4 modelMat;

void main()
{
	gl_Position = lightProjMat * modelMat * vec4(aPos, 1.0f);
}