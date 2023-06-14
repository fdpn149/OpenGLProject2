#version 410 core

struct Light
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

uniform vec3 cameraPos;

void main()
{
	gl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}