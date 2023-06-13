#version 410 core

in vec2 texcoord;

out vec4 FragColor;

uniform sampler2D Texture;
uniform bool UseTexture;

void main()
{
	if(UseTexture)
	{
		FragColor = texture(Texture, texcoord);
	}
	else
	{
		FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
}