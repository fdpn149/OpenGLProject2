#version 410 core

uniform vec3 faceColor;
out vec4 fragColor;

void main(void) 
{
	fragColor = vec4(faceColor,1.0);
}