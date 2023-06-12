#version 410 core

in vec2 texcoord;
flat in vec3 color;
in float useTexture;

out vec4 FragColor;

uniform sampler2D Texture;

void main()
{
	if(useTexture > 0.5f)
	{
		FragColor = texture(Texture, texcoord);
	}
	else
	{
		FragColor = vec4(color, 1.0f);
	}
}