#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aColor;
layout (location = 3) in int aUseTex;

out vec2 texcoord;
flat out vec3 color;
out float useTexture;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
	gl_Position = projMat * viewMat * modelMat * vec4(aPos, 1.0);

	texcoord= aTexCoord;
	color = aColor;
	useTexture = float(aUseTex);
}