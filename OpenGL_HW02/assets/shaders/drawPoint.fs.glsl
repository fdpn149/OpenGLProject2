#version 410 core

uniform vec3 pointColor;
out vec4 fragColor;

void main(void) 
{
	fragColor = vec4(pointColor,1.0);
}