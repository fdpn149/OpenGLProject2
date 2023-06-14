#version 410 core

in vec3 texcoord;

uniform samplerCube skybox;

void main()
{    
    gl_FragColor = texture(skybox, texcoord);
}