#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform bool enableFilter;

const float offset = 1.0 / 600.0;  

void main()
{
    if(enableFilter)
    {
        vec2 offsets[9] = vec2[](
            vec2(-offset,  offset),
            vec2( 0.0f,    offset),
            vec2( offset,  offset),
            vec2(-offset,  0.0f),  
            vec2( 0.0f,    0.0f),  
            vec2( offset,  0.0f),  
            vec2(-offset, -offset),
            vec2( 0.0f,   -offset),
            vec2( offset, -offset) 
        );

        float kernel_x[9] = float[](
            -1,  0,  1,
            -2,  0,  2,
            -1,  0,  1
        );

        float kernel_y[9] = float[](
             1,  2,  1,
             0,  0,  0,
            -1, -2, -1
        );

        vec3 sampleTex[9];
        vec4 fragcolor;
        float average;
        for(int i = 0; i < 9; i++)
        {
            fragcolor = texture(screenTexture, TexCoords.st + offsets[i]);
            average = 0.2126 * fragcolor.r + 0.7152 * fragcolor.g + 0.0722 * fragcolor.b;
            sampleTex[i] = vec3(average);
        }
        vec3 color_x = vec3(0.0);
        vec3 color_y = vec3(0.0);
        for(int i = 0; i < 9; i++)
        {
            color_x += sampleTex[i] * kernel_x[i];
            color_y += sampleTex[i] * kernel_y[i];
        }

        color_x *= color_x;
        color_y *= color_y;

        vec3 color = sqrt(color_x + color_y);

        FragColor = vec4(color, 1.0);
    }
    else
    {
        FragColor = texture(screenTexture, TexCoords);
    }
}