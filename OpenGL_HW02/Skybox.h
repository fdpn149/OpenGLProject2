#pragma once

#include <string>

#include "Shader.h"

class Skybox
{
public:
	Skybox() = default;
	Skybox(const std::string& directory);

	void loadTexture(const std::string& directory);

	void draw(Shader& shader);

private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int texId;
};