#include "pch.h"
#include "TextureParamPainter.h"

#include <iostream>

#include <GL/glew.h>

#include <STB/stb_image.h>

#include "utilities.h"
#include "Config.h"
#include "ConvexCombMap.h"

TextureParamPainter::TextureParamPainter()
{
	myShader = Shader(Config::SHADER_PATH + "textureParam.vs.glsl", Config::SHADER_PATH + "textureParam.fs.glsl");

	loadTexture(Config::TEXTURE_PATH + "container.jpg");

	std::vector<unsigned int> test = { 0, 1, 2 };

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

		std::cout << "x: " << texcoord.x << " y: " << texcoord.y << std::endl;

		this->vertices.push_back(texcoord);
	}

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vertVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, texVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texcoords.size(), texcoords.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void TextureParamPainter::draw()
{
	if (!texcoords.empty())
	{
		glBindVertexArray(vao);

		myShader.use();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw texture
		myShader.setInt("UseTexture", 1);

		myShader.setInt("Texture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texId);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		std::cout << glGetError() << std::endl;

		// Draw lines
		myShader.setInt("UseTexture", 0);

		glBindBuffer(GL_ARRAY_BUFFER, vertVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * lineVertices.size(), lineVertices.data(), GL_STATIC_DRAW);
		glLineWidth(1.0f);
		glDrawArrays(GL_LINES, 0, lineVertices.size());

	}
	else
	{
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	glBindVertexArray(0);
}

void TextureParamPainter::loadTexture(const std::string& path)
{

	// initialize OpenGL texture
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	// configure the type of algorithm for the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// configure the way to repeat texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	stbi_set_flip_vertically_on_load(false);

	// load image
	int width, height, channelNum;
	unsigned char* imageBytes = stbi_load(path.c_str(), &width, &height, &channelNum, 0);
	if (imageBytes)
	{
		GLenum format;
		if (channelNum == 1)
			format = GL_RED;
		else if (channelNum == 3)
			format = GL_RGB;
		else if (channelNum == 4)
			format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageBytes);

		stbi_image_free(imageBytes);
	}
	else
	{
		std::cout << "Cubemap tex failed to load at path: " << std::endl;
		stbi_image_free(imageBytes);
	}

	// generate mipmap
	glGenerateMipmap(GL_TEXTURE_2D);

	// unbind texture object
	glBindTexture(GL_TEXTURE_2D, 0);
}
