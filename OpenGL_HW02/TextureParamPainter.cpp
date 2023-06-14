#include "pch.h"
#include "TextureParamPainter.h"

#include <iostream>

#include <GL/glew.h>

#include <STB/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/reciprocal.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "utilities.h"
#include "Config.h"
#include "ConvexCombMap.h"


TextureParamPainter::TextureParamPainter()
	: texId(0)
	, drawTexture(false)
	, textureUpdated(true)
{
	glDisable(GL_CULL_FACE);

	myShader = Shader(Config::SHADER_PATH + "textureParam.vs.glsl", Config::SHADER_PATH + "textureParam.fs.glsl");

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vertVbo);
	glGenBuffers(1, &texVbo);
	glGenBuffers(1, &ebo);
}

void TextureParamPainter::setTexture(const std::string& file)
{
	if (texId != 0)
	{
		glDeleteTextures(1, &texId);
	}

	if (!file.empty())
	{
		texId = Utils::loadTexture(file);
	}

	drawTexture = true;
}

void TextureParamPainter::setLineData(const std::vector<glm::vec2> lineVertices)
{
	this->lineVertices = lineVertices;
}

void TextureParamPainter::setMesh2DData(const Mesh& mesh)
{
	vertices.clear();
	indices = mesh.getSelectedIndices();

	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(1.25f, 1.25f, 1.25f));

	for (const auto& vertex : mesh.getSelectedVertices())
	{
		glm::vec2 texcoord = ConvexCombMap::map(Utils::toGlmVec3(vertex));
		texcoords.push_back(texcoord);

		vertices.push_back(Utils::texcoordToCartesian(texcoord));
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
		if (texId != 0 && drawTexture)
		{
			myShader.setInt("UseTexture", 1);

			myShader.setInt("Texture", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texId);

			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vertVbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
		
	}
	else
	{
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}
