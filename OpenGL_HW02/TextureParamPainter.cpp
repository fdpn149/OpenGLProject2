#include "pch.h"
#include "TextureParamPainter.h"

#include <iostream>

#include <GL/glew.h>

#include <STB/stb_image.h>

#include "utilities.h"
#include "Config.h"
#include "ConvexCombMap.h"

namespace
{
	std::vector<glm::vec2> quadVertices = { glm::vec2(-1, -1), // bottom left corner
											glm::vec2(-1,  1), // top left corner
											glm::vec2( 1,  1), // top right corner
											glm::vec2( 1, -1)}; // bottom right corner


	std::vector<unsigned int> quadIndices = { 0, 1, 2, // first triangle (bottom left - top left - top right)
											  0, 2, 3 }; // second triangle (bottom left - top right - bottom right)

}

TextureParamPainter::TextureParamPainter()
{
	glDisable(GL_CULL_FACE);

	myShader = Shader(Config::SHADER_PATH + "textureParam.vs.glsl", Config::SHADER_PATH + "textureParam.fs.glsl");

	texId = Utils::loadTexture(Config::TEXTURE_PATH + "container.jpg");

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vertVbo);
	glGenBuffers(1, &texVbo);
	glGenBuffers(1, &ebo);
}

void TextureParamPainter::setLineData(const std::vector<glm::vec2> lineVertices)
{
	this->lineVertices = lineVertices;
}

void TextureParamPainter::setMesh2DData(const Mesh& mesh)
{
	vertices.clear();
	indices = mesh.getSelectedIndices();

	for (const auto& vertex : mesh.getSelectedVertices())
	{
		glm::vec2 texcoord = ConvexCombMap::map(Utils::toGlmVec3(vertex));
		this->texcoords.push_back(texcoord);

		texcoord.x = texcoord.x * 2 - 1;
		texcoord.y = texcoord.y * 2 - 1;

		this->vertices.push_back(texcoord);
	}

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vertVbo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, texVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texcoords.size(), texcoords.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
}

void TextureParamPainter::draw()
{
	if (!texcoords.empty())
	{
		myShader.use();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw lines
		myShader.setInt("UseTexture", 0);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vertVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * lineVertices.size(), lineVertices.data(), GL_STATIC_DRAW);

		glLineWidth(1.0f);
		glDrawArrays(GL_LINES, 0, lineVertices.size());

		// Draw texture
		myShader.setInt("UseTexture", 1);

		myShader.setInt("Texture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texId);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vertVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
	}
	else
	{
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}
