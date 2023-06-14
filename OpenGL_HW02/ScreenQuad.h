#pragma once

#include "Shader.h"

class ScreenQuad
{
public:
	ScreenQuad();

	void draw(Shader& shader);

private:
	void initBufferObjects();

private:
	unsigned int vao;
	unsigned int vbo;
};

