#pragma once

#include "Shader.h"

class Plane
{
public:
	Plane();

	void draw(Shader& shader);

private:
	void initBufferObjects();

private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;

	unsigned int diffuseTexId;
	unsigned int specularTexId;
	//unsigned int normalTexId;
};

