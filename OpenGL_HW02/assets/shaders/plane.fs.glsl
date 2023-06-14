#version 410 core

in vec3 position;
in vec3 normal;
in vec2 texcoord;

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

uniform vec3 cameraPos;


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
    

    vec3 result = ambient + diffuse + specular;
    gl_FragColor = vec4(result, 1.0f);
}