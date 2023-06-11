#version 410 core

in vec2 texcoord;

out vec4 fragColor;

uniform bool UseTexture;
uniform sampler2D Texture;
uniform vec3 faceColor;

void main(void) 
{
	if(UseTexture)
	{
		fragColor = texture(Texture, texcoord);
	}
	else
	{
		fragColor = vec4(faceColor, 1.0f);
	}
}