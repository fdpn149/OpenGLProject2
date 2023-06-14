#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "Shader.h"

class Light
{
public:
	Light(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
		: direction(direction), ambient(ambient), diffuse(diffuse), specular(specular)
	{

	}

	void setLight2Shader(Shader& shader);

private:
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

