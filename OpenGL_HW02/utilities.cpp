#include "pch.h"
#include "utilities.h"

#include <GL/glew.h>
#include <STB/stb_image.h>

glm::vec3 Utils::toGlmVec3(TriMesh::Point point)
{
	return glm::vec3(point[0], point[1], point[2]);
}

unsigned int Utils::loadTexture(const std::string& file)
{
	unsigned int id;

	// initialize OpenGL texture
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	// configure the type of algorithm for the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// configure the way to repeat texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	stbi_set_flip_vertically_on_load(false);

	// load image
	int width, height, channelNum;
	unsigned char* imageBytes = stbi_load(file.c_str(), &width, &height, &channelNum, 0);
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

	return id;
}

glm::vec2 Utils::texcoordToCartesian(const glm::vec2 position)
{
	return glm::vec2(position.x * 2 - 1, position.y * 2 - 1);
}

glm::vec2 Utils::cartesianToTexcoord(const glm::vec2 position)
{
	return glm::vec2((position.x + 1) / 2, (position.y + 1) / 2);
}
