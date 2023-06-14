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


/*                 JSON                     */
#include <nlohmann/json.hpp>
using json = nlohmann::json;



/*                 My Class                 */
#include "Shader.h"
#include "MeshData.h"


/*                 Type def                 */
typedef OpenMesh::TriMesh_ArrayKernelT<> TriMesh;


class Mesh
{	
public:
	Mesh();
	Mesh(const std::string& file);

	/* Load model */

	void loadMesh(const std::string& file);


	/* Save&load system */

	void saveSelectedAsJson(const std::string& file);
	void loadSelectedFromJson(const std::string& file);


	/* Getters */

	const TriMesh& getSelectedMeshRef() const { return selectedMesh; }
	const std::vector<TriMesh::Point>& getSelectedVertices() const { return (*(selectedMeshData.end() - 1)).vertices; }
	const std::vector<glm::vec2>& getSelectedTexcoords() const { return (*(selectedMeshData.end() - 1)).texcoords; }
	const std::vector<unsigned int>& getSelectedIndices() const { return (*(selectedMeshData.end() - 1)).indices; }
	unsigned int getTexIdByIdx(int idx) { return textureIds.at(idx); }
	int getTextureNum() { return textureIds.size(); }
	glm::mat4 getModelMat() { return modelMat; }


	/* Setters */

	void setTexture(const std::string& file);
	void setTexcoord();
	void setNewSelectMesh();


	/* Draw methods */

	void draw();
	void drawSelected(Shader& shader);
	void drawPoint();


	void addVertex(int faceID, glm::vec3 worldPos);
	void addFace(int faceId);

	void addFaceToSelectedById(int faceId);
	void deleteFaceFromSelectedById(int faceId);


	/* Pick vertex methods*/
	TriMesh::VertexHandle findClosestPoint(int faceID, glm::vec3 worldPos);

	void setPointPosition(glm::vec3 position);


private:
	void updateSelectedBufferObjects();
	void updateSelectedFVMap(int face_3verticesID[]);

	void initSelectedBufferObjs();
	void loadSelectedBufferObjs();


private:
	TriMesh modelMesh;
	TriMesh selectedMesh;

	std::map<int, int> selectedModelFaceMap;
	std::map<int, int> selectedModelVertMap;

	OpenMesh::FPropHandleT<int> faceIdPropHanlde;
	OpenMesh::VPropHandleT<int> vertIdPropHandle;


	std::vector<TriMesh::Point> modelVertices;
	std::vector<unsigned int> modelIndices;

	std::vector<MeshData::SelectedMeshData> selectedMeshData;
	std::vector<MeshData::SelectedTextureData> selectedTextureData;

	glm::mat4 modelMat;

	std::vector<TriMesh::Point> lines;

	unsigned int selectedVertVbo;

	unsigned int modelVbo;
	unsigned int modelVao;
	unsigned int modelEbo;

	std::vector<unsigned int> textureIds;

	unsigned int selectedTexId;
};