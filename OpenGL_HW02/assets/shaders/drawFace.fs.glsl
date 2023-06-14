#version 410 core

in vec2 texcoord;

uniform bool UseTexture;
uniform sampler2D Texture;

void main(void) 
{
	if(UseTexture)
	{
		gl_FragColor = texture(Texture, texcoord);
	}
	else
	{
		gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
}