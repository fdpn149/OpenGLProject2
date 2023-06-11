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

	const TriMesh& getSelectedMeshRef() const { return selectedMesh; }
	const std::vector<TriMesh::Point>& getSelectedVertices() const { return selectedVertices; }
	const std::vector<unsigned int>& getSelectedIndices() const { return selectedIndices; }

	void draw();
	void drawSelecetedFaces();
	void drawPoint();
	void drawLine();	//For Debug

	void calcTexcoord();

	void addFaceToSelectedById(int faceId);
	void deleteFaceFromSelectedById(int faceId);

	TriMesh::Point findClosestPoint(uint faceID, glm::vec3 worldPos);
	void setPointPosition(glm::vec3 position);
	void setLinePosition();

private:
	void updateSelectedBufferObjects();
	void updateSelectedFVMap(int face_3verticesID[]);


private:
	TriMesh modelMesh;
	TriMesh selectedMesh;

	std::map<int, int> selectedModelFaceMap;
	std::map<int, int> selectedModelVertMap;

	OpenMesh::FPropHandleT<int> faceIdPropHanlde;
	OpenMesh::VPropHandleT<int> vertIdPropHandle;

	std::vector<TriMesh::Point> selectedVertices;
	std::vector<glm::vec2> selectedTexcoords;
	std::vector<unsigned int> selectedIndices;

	glm::mat4 modelMat;

	std::vector<TriMesh::Point> lines;

	unsigned int selectedVertVbo;

	unsigned int modelVbo;
	unsigned int modelVao;
	unsigned int modelEbo;

	unsigned int selectedVbo;
	unsigned int selectedTexVbo;
	unsigned int selectedVao;
	unsigned int selectedEbo;

	unsigned int selectedTexId;

	unsigned int vao3, vbo3;	//Draw Line (For Debug)
};