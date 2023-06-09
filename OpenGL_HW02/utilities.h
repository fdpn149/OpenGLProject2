#pragma once

/*                 OpenMesh                 */
#define _USE_MATH_DEFINES
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriConnectivity.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

/*                 Glm                      */
#include <glm/glm.hpp>

/*                 Type Defs                */
typedef OpenMesh::TriMesh_ArrayKernelT<> TriMesh;

namespace Utils
{
	// Convert TriMesh point to glm::vec3
	glm::vec3 toGlmVec3(TriMesh::Point point);

	// Get sign of a num
	template<typename T>
	int sgn(T val)
	{
		return (T(0) < val) - (val < T(0));
	}

}