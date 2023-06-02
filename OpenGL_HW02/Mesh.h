#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <set>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

#include "FaceGroup.h"

class Shader;

class Mesh : public OpenMesh::TriMesh_ArrayKernelT<>
{

	glm::mat4 modelMat;

	//std::set<uint> selectedFace;

	uint vert_vbo;

	uint vbo, vao, ebo;
public:
	Mesh();
	void draw();
	void drawFace(FaceGroup& faceGroup);
	//void drawFace();
	void drawPoint();

	//void addSelectedFace(uint faceID);
	//void deleteSelectedFace(uint faceID);

	void setPointPosition(glm::vec3 position);
};