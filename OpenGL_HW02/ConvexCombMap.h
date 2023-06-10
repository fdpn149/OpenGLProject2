#pragma once

#include "Mesh.h"

/*                 Glm                      */
#include <glm/glm.hpp>

/*                 Type Defs                */
typedef OpenMesh::TriMesh_ArrayKernelT<> TriMesh;

namespace ConvexCombMap
{
	void calc(const TriMesh& mesh);
	glm::vec2 map(const glm::vec3& vertex);
	int mapNum();
}