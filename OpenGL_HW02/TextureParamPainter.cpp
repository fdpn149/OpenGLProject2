#include "pch.h"
#include "TextureParamPainter.h"

#include <iostream>

#include <GL/glew.h>

#include "Config.h"

TextureParamPainter::TextureParamPainter()
{
	myShader = Shader(Config::SHADER_PATH + "textureParam.vs.glsl", Config::SHADER_PATH + "textureParam.fs.glsl");

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TextureParamPainter::setTexcoords(const std::vector<glm::vec2>& texcoords)
{
	this->texcoords = texcoords;
}

void TextureParamPainter::draw()
{
	glBindVertexArray(vao);

	if (!texcoords.empty())
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * this->texcoords.size(), this->texcoords.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		myShader.use();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(1.0f);
		glDrawArrays(GL_LINES, 0, texcoords.size());
	}
	else
	{
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	glBindVertexArray(0);
}
