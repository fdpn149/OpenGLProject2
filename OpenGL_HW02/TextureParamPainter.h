#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Shader.h"

class TextureParamPainter
{
public:
	TextureParamPainter();

	void setTexcoords(const std::vector<glm::vec2>& texcoords);

	void draw();

private:
	unsigned int vao;
	unsigned int vbo;

	std::vector<glm::vec2> texcoords;

	Shader myShader;
};

