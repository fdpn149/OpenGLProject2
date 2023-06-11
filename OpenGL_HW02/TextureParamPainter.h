#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Shader.h"
#include "Mesh.h"

class TextureParamPainter
{
public:
	TextureParamPainter();

	void setLineData(const std::vector<glm::vec2> lineVertices);
	void setMesh2DData(const Mesh& mesh);

	void draw();

private:
	void loadTexture(const std::string& path);

private:
	unsigned int vao;
	unsigned int ebo;
	unsigned int vertVbo;
	unsigned int texVbo;

	unsigned int texId;

	std::vector<glm::vec2> lineVertices;

	std::vector<glm::vec2> vertices;
	std::vector<unsigned int> indices;
	std::vector<glm::vec2> texcoords;

	Shader myShader;
};

