#pragma once

#include <glm/glm.hpp>
#include <set>

class FaceGroup
{
public:
	FaceGroup();

	void setColor(glm::vec3 newColor);
	void addFace(unsigned int faceId);
	void deleteFace(unsigned int faceId);

	std::set<unsigned int> getFaceIds() { return selectedFaces; }
	glm::vec3 getColor() { return color; }

private:
	std::set<unsigned int> selectedFaces;
	glm::vec3 color;
};