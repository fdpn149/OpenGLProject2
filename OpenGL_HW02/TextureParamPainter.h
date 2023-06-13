#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Shader.h"
#include "Mesh.h"

class TextureParamPainter
{
public:
	TextureParamPainter();

	bool isUpdatedTexture() { return textureUpdated; }

	void setTextureUpdatedState(bool updated) { textureUpdated = updated; }
	void setTexture(const std::string& file);
	void setLineData(const std::vector<glm::vec2> lineVertices);
	void setMesh2DData(const Mesh& mesh);

	void draw();

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

	bool textureUpdated;
};

