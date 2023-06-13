#pragma once
/*                 Standard                 */
#include <iostream>
#include <string>

/*                 OpenMesh                 */
#define _USE_MATH_DEFINES
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriConnectivity.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

/*                 Glm                      */
#include <glm/glm.hpp>


/*                 DonNet                   */
#include <msclr\marshal_cppstd.h>


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

	// Load texture
	unsigned int loadTexture(const std::string& file);

	glm::vec2 texcoordToCartesian(const glm::vec2 position);
	glm::vec2 cartesianToTexcoord(const glm::vec2 position);

	std::string donNetStringToSTLString(System::String^ str);
}