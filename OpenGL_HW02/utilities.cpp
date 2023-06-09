#include "pch.h"
#include "utilities.h"

glm::vec3 Utils::toGlmVec3(TriMesh::Point point)
{
	return glm::vec3(point[0], point[1], point[2]);
}
