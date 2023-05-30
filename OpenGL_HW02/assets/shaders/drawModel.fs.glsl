#version 410 core
out vec4 FragColor;

in vec3 aColor;

void main()
{
    FragColor = vec4(aColor + 0.5, 1.0);
}