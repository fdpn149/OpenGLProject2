#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriConnectivity.hh>
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

	std::vector<TriMesh::Point> lines;

	std::map<TriMesh::Point, TriMesh::Point> point3D_2D;

	TriMesh selected;

	uint vert_vbo;
	uint vbo, vao, ebo;
	uint vao2, vbo2;

	uint vao3, vbo3;

	glm::vec3 pointToVec3(const TriMesh::Point& point);
	TriMesh::Point vec3ToPoint(const glm::vec3 vec);
	TriMesh::Point percentToXY(float percent);
public:
	Mesh();
	void draw();
	void drawFace();
	void drawPoint();
	void drawLine();
	void drawSelected();

	void addSelectedFace(uint faceID);
	void deleteSelectedFace(uint faceID);

	TriMesh::Point findClosestPoint(uint faceID, glm::vec3 worldPos);
	void setPointPosition(glm::vec3 position);
	void setLinePosition();

	void calculateSurround(std::vector<TriMesh::Point>& points);
	void calculateInside(std::vector<TriMesh::Point>& points);
};