#pragma once

/*                 Standard                 */
#include <iostream>
#include <set>
#include <map>
#include <string>


/*                 GLM                      */
#include <glm/glm.hpp>


/*                 OpenMesh                 */
#define _USE_MATH_DEFINES
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriConnectivity.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>


/*                 My Class                 */
#include "Shader.h"


/*                 Type def                 */
typedef OpenMesh::TriMesh_ArrayKernelT<> TriMesh;


class Mesh
{
public:
	Mesh();
	Mesh(const std::string& file);

	void load(const std::string& file);

	void draw();
	void drawFaceByIds(std::set<unsigned int> faceIds);
	void drawPoint();
	void drawLine();	//For Debug

	TriMesh::Point findClosestPoint(uint faceID, glm::vec3 worldPos);
	void setPointPosition(glm::vec3 position);
	void setLinePosition();

	std::vector<TriMesh::Point> getFaceVerticesById(unsigned int faceId);


private:
	TriMesh mesh;
	glm::mat4 modelMat;

	std::vector<TriMesh::Point> lines;

	unsigned int vert_vbo;
	unsigned int vbo, vao, ebo;
	unsigned int vao2, vbo2;

	unsigned int vao3, vbo3;	//Draw Line (For Debug)
};