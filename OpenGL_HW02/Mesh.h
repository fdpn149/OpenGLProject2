#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <set>
#include <map>

typedef OpenMesh::TriMesh_ArrayKernelT<> TriMesh;

class Shader;

class Mesh
{
	TriMesh model;
	glm::mat4 modelMat;
	std::set<uint> selectedFace;
		

	TriMesh selected;
	//std::vector<TriMesh::Point> select_vertices;

	uint vert_vbo;
	uint vbo, vao, ebo;
	uint vao2, vbo2;
public:
	Mesh();
	void draw();
	void drawFace();
	void drawPoint();
	void drawSelected();

	void addSelectedFace(uint faceID);
	void deleteSelectedFace(uint faceID);

	TriMesh::Point findClosestPoint(uint faceID, glm::vec3 worldPos);
	void setPointPosition(glm::vec3 position);

	void calculateSurround(std::vector<float>& percent);
};