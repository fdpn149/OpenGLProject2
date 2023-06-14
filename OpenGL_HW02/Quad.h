#pragma once
#include "Shader.h"

class Quad
{
public:
	Quad();

	void draw(Shader& shader);

	void setTexId(unsigned int id) { diffuse = id; }

private:
	void initBufferObjects();

private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;

	unsigned int diffuse;
	unsigned int specular;
};