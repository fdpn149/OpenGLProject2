#include "pch.h"
#include "Light.h"

void Light::setLight2Shader(Shader& shader)
{
	shader.setVec3("light.direction", direction);
	shader.setVec3("light.ambient", ambient);
	shader.setVec3("light.diffuse", diffuse);
	shader.setVec3("light.specular", specular);
}
