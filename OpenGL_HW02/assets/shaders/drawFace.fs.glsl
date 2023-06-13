#version 410 core

in vec2 texcoord;

out vec4 fragColor;

uniform bool UseTexture;
uniform sampler2D Texture;

void main(void) 
{
	if(UseTexture)
	{
		fragColor = texture(Texture, texcoord);
	}
	else
	{
		fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
}