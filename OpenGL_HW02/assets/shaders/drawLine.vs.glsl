#version 410 core

layout(location = 0) in vec3 vertex;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
	gl_Position = vec4(vertex,1.0);
}
