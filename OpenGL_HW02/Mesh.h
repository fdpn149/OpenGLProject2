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

	std::map<int, int> face_model_selected;
	std::map<int, int> vertex_model_selected;

	std::vector<TriMesh::Point> lines;

	std::map<TriMesh::Point, TriMesh::Point> boundary_point3D_2D;	//convert world point to 2d xy point (0.0f~1.0f)
	std::map<TriMesh::Point, TriMesh::Point> inside_point3D_2D;

	TriMesh selected;

	OpenMesh::FPropHandleT<int> faceID_about_model;	//add correspond model faceID to selected
	OpenMesh::VPropHandleT<int> vertexID_about_model;

	uint vert_vbo;
	uint vbo, vao, ebo;
	uint vao2, vbo2;

	uint vao3, vbo3;	//Draw Line (For Debug)

	glm::vec3 pointToVec3(const TriMesh::Point& point);
	TriMesh::Point percentToXY(float percent);

	std::vector<TriMesh::VertexHandle> addVerticesToSelected(const TriMesh::FaceHandle& fh_model);

	void updateBuffer();
	void updateFVSelectedMap(int face_3verticesID[]);
public:
	Mesh();
	void draw();
	void drawFace();	//Useless
	void drawPoint();
	void drawLine();	//For Debug
	void drawSelected();

	void addSelectedFace(int faceID);
	void deleteSelectedFace(int faceID);

	TriMesh::Point findClosestPoint(int faceID, glm::vec3 worldPos);
	void setPointPosition(glm::vec3 position);
	void setLinePosition();

	void calculateSurround(std::vector<TriMesh::Point>& points);
	void calculateInside(std::vector<std::vector<TriMesh::Point>>& points);
};