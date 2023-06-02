#include "pch.h"
#include "FaceGroup.h"

FaceGroup::FaceGroup()
{
	selectedFaces.clear();
	color = glm::vec3(1.0f, 0.0f, 0.0f);
}

void FaceGroup::setColor(glm::vec3 newColor)
{
	color = newColor;
}

void FaceGroup::addFace(unsigned int faceId)
{
	selectedFaces.insert(faceId);
}

void FaceGroup::deleteFace(unsigned int faceId)
{
	selectedFaces.erase(faceId);
}
