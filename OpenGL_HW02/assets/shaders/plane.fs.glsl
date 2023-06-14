#version 410 core

in vec3 position;
in vec3 normal;
in vec2 texcoord;
in vec4 fragPosLight;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;

uniform Light light;

uniform sampler2D shadowMap;

uniform vec3 cameraPos;

float calculateShadow(vec4 fragPosLight)
{
    vec3 projCoords = fragPosLight.xyz / fragPosLight.w;

    projCoords = projCoords * 0.5 + 0.5;

    float closetDepth = texture(shadowMap, projCoords.xy).r;

    float currentDepth = projCoords.z;

    float bias = max(0.05 * (1.0 - dot(normal, normalize(-light.direction))), 0.005);

    float shadow = 0.0f;

    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }

    shadow /= 9.0;

    return shadow;
}


void main()
{
	vec3 lightDir = normalize(-light.direction);

    // Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texcoord));

    // Diffuse
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.0);
    
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texcoord));

    // Specular
    vec3 viewDir= normalize(cameraPos - position);
    vec3 reflectionDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectionDir), 0.0f), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texcoord));
    
    float shadow = calculateShadow(fragPosLight);

    vec3 result = (ambient + (1.0f - shadow) * (diffuse + specular));
    gl_FragColor = vec4(result, 1.0f);
}