#include "pch.h"
#include "Mesh.h"

/*                 Standard                 */
#include <iostream>
#include <fstream>
#include <set>
#include <map>


/*                 OpenGL                   */
#include <GL/glew.h>


/*                 GLM                      */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "util_glm.hpp"


/*                 Eigen                    */
#include <eigen3/Eigen/Dense>


/*                 JSON                     */
#include <nlohmann/json.hpp>
using json = nlohmann::json;


/*                 My Class                 */
#include "Shader.h"
#include "utilities.h"
#include "Config.h"
#include "ConvexCombMap.h"

namespace VertexObjType
{
	enum VertexObjType
	{
		POSITION,
		TEXCOORD,
		MAX_NUM,
	};
}

Mesh::Mesh()
	: modelMat(glm::mat4(1.0f))
{
	selectedMesh.add_property(faceIdPropHanlde);
	selectedMesh.add_property(vertIdPropHandle);

	setNewSelectMesh();
}

Mesh::Mesh(const std::string& file)
{
	loadMesh(file);
	Mesh();
}

void Mesh::loadMesh(const std::string& file)
{
	if (OpenMesh::IO::read_mesh(modelMesh, file))
	{
		/* OpenMesh */

		modelMesh.request_vertex_status();
		modelMesh.request_edge_status();
		modelMesh.request_face_status();
		modelMesh.request_face_normals();
		modelMesh.update_normals();


		/* Get vertices */
		modelVertices.reserve(modelMesh.n_vertices());

		for (TriMesh::VertexIter v_it = modelMesh.vertices_begin(); v_it != modelMesh.vertices_end(); ++v_it)
		{
			modelVertices.push_back(modelMesh.point(*v_it));
		}


		/* Get indices */

		modelIndices.reserve(modelMesh.n_faces() * 3);
		for (TriMesh::FaceIter f_it = modelMesh.faces_begin(); f_it != modelMesh.faces_end(); ++f_it)
		{
			for (TriMesh::FaceVertexIter fv_it = modelMesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
			{
				modelIndices.push_back((*fv_it).idx());
			}
		}

		/* Initialize Model Buffer Objects */

		// VAO
		glGenVertexArrays(1, &modelVao);
		glBindVertexArray(modelVao);


		// VBO
		glGenBuffers(1, &modelVbo);
		glBindBuffer(GL_ARRAY_BUFFER, modelVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TriMesh::Point) * modelVertices.size(), modelVertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TriMesh::Point), (void*)0);


		// EBO
		glGenBuffers(1, &modelEbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelEbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * modelIndices.size(), modelIndices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
	}
	else
	{
		std::cout << "Failed to load mesh." << std::endl;
	}
}

void Mesh::saveSelectedAsJson(const std::string& file)
{
	std::ofstream out(file);

	json savedJson;
	savedJson["mesh"] = selectedMeshData;
	savedJson["texture"] = selectedTextureData;

	out << std::setw(4) << savedJson << std::endl;

	out.close();

	OpenMesh::IO::write_mesh(selectedMesh, Config::MODEL_PATH + "saves.obj");
}

void Mesh::loadSelectedFromJson(const std::string& file)
{
	std::ifstream in(file);

	json savedJson;
	in >> savedJson;

	/* Load vertex data */

	std::vector<MeshData::SelectedMeshData> selectedMeshDataBuffer;
	savedJson.at("mesh").get_to(selectedMeshDataBuffer);

	selectedMeshData.clear();
	for (const auto& data : selectedMeshDataBuffer)
	{
		selectedMeshData.push_back(data);

		loadSelectedBufferObjs();
	}


	/* Load texture data */

	for (auto& textureData : selectedTextureData)
	{
		glDeleteTextures(1, &textureData.id);
	}

	selectedTextureData.clear();
	savedJson.at("texture").get_to(selectedTextureData);

	for (auto& textureData : selectedTextureData)
	{
		textureData.id = Utils::loadTexture(textureData.file);
	}

	setNewSelectMesh();

	std::cout << selectedMeshData.size() << '|' << selectedTextureData.size() << std::endl;

	in.close();
}

void Mesh::setTexture(const std::string& file)
{
	if ((*(selectedTextureData.end() - 1)).id != 0)
	{
		glDeleteTextures(1, &((*(selectedTextureData.end() - 1)).id));
	}

	unsigned id = Utils::loadTexture(file);

	(*(selectedTextureData.end() - 1)).id = id;
	(*(selectedTextureData.end() - 1)).file = file;

	(*(selectedMeshData.end() - 1)).useTexture = true;
}

void Mesh::setTexcoord()
{
	MeshData::SelectedMeshData& lastSelectedMeshRef = (*(selectedMeshData.end() - 1));

	int idx = 0;
	for (auto v_it = selectedMesh.vertices_begin(); v_it != selectedMesh.vertices_end(); ++v_it)
	{
		lastSelectedMeshRef.texcoords[idx++] = ConvexCombMap::map(Utils::toGlmVec3(selectedMesh.point(*v_it)));
	}

	glBindBuffer(GL_ARRAY_BUFFER, lastSelectedMeshRef.vbo[VertexObjType::TEXCOORD]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * lastSelectedMeshRef.texcoords.size(), lastSelectedMeshRef.texcoords.data(), GL_DYNAMIC_DRAW);
}

void Mesh::draw()
{
	glBindVertexArray(modelVao);
	glDrawElements(GL_TRIANGLES, modelMesh.n_faces() * 3, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}

void Mesh::drawSelected(Shader& shader)
{
	for (int iii = 0; iii != selectedMeshData.size(); ++iii)
	{
		shader.use();

		shader.setInt("Texture", 0);
		shader.setInt("UseTexture", selectedMeshData[iii].useTexture);

		if (selectedMeshData[iii].useTexture)
		{
			glBindTexture(GL_TEXTURE_2D, selectedTextureData[iii].id);
		}

		glBindVertexArray(selectedMeshData[iii].vao);
		glDrawElements(GL_TRIANGLES, selectedMeshData[iii].indices.size(), GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);
	}
}

void Mesh::drawPoint()
{
	glPointSize(15.0f);
	glDrawArrays(GL_POINTS, 0, 1);
}

void Mesh::setNewSelectMesh()
{
	selectedMeshData.push_back({ {}, {}, {}, false, 0, 0, 0 });
	selectedTextureData.push_back({ 0, "" });

	std::cout << selectedMeshData.size() << '|' << selectedTextureData.size() << std::endl;

	initSelectedBufferObjs();

	selectedMesh.clear();
	selectedMesh.garbage_collection();

	selectedModelFaceMap.clear();
	selectedModelVertMap.clear();
}

void Mesh::addFaceToSelectedById(int faceId)
{
	if (faceId >= 0 && faceId < modelMesh.n_faces())
	{
		TriMesh::FaceHandle modelSelectedFh = modelMesh.face_handle(faceId);

		std::vector<TriMesh::VertexHandle> faceVertHandles;

		// Check if it was selected
		if (!modelMesh.status(modelSelectedFh).selected())
		{
			modelMesh.status(modelSelectedFh).set_selected(true);

			// Loop face vertices
			for (auto fv_it = modelMesh.fv_begin(modelSelectedFh); fv_it.is_valid(); fv_it++)
			{
				int vertexId = fv_it->idx();

				TriMesh::VertexHandle vh;

				// Check if the vertex is already in selected mesh
				if (selectedModelVertMap.find(vertexId) != selectedModelVertMap.end())
				{
					vh = selectedMesh.vertex_handle(selectedModelVertMap[vertexId]);
				}
				// Add non-exist vertex to selected mesh
				else
				{
					TriMesh::Point newVertexPos = modelMesh.point(*fv_it);
					vh = selectedMesh.add_vertex(newVertexPos);

					selectedMesh.property(vertIdPropHandle, vh) = vertexId + 1;

					selectedModelVertMap[fv_it->idx()] = vh.idx();
				}

				faceVertHandles.push_back(vh);
			}


			TriMesh::FaceHandle newSelectedFh = selectedMesh.add_face(faceVertHandles);

			selectedMesh.property(faceIdPropHanlde, newSelectedFh) = faceId + 1;

			selectedModelFaceMap[faceId] = newSelectedFh.idx();

			updateSelectedBufferObjects();
		}
	}
}

void Mesh::deleteFaceFromSelectedById(int faceId)
{
	TriMesh::FaceHandle fh_in_model = modelMesh.face_handle(faceId);

	if (!fh_in_model.is_valid() || !modelMesh.status(fh_in_model).selected())
		return;

	modelMesh.status(fh_in_model).set_selected(false);
	int id = selectedModelFaceMap[faceId];

	TriMesh::FaceHandle fh_in_selected = selectedMesh.face_handle(id);

	if (!fh_in_selected.is_valid())
		fprintf(stderr, "Error\n");

	int face_3verticesID[3] = { 0 };

	int index = 0;
	for (auto fv_it = selectedMesh.fv_begin(fh_in_selected); fv_it.is_valid(); fv_it++)
	{
		face_3verticesID[index] = selectedMesh.property(vertIdPropHandle, *fv_it) - 1;
		index++;
	}

	selectedMesh.delete_face(fh_in_selected);

	selectedModelFaceMap.erase(faceId);

	selectedMesh.garbage_collection();

	updateSelectedFVMap(face_3verticesID);

	updateSelectedBufferObjects();
}

TriMesh::Point Mesh::findClosestPoint(uint faceID, glm::vec3 worldPos)
{
	OpenMesh::FaceHandle fh = modelMesh.face_handle(faceID);
	if (!fh.is_valid())
	{
		printf("invalid\n");
		return TriMesh::Point();
	}

	double minDistance = 0.0;
	TriMesh::Point p(worldPos.x, worldPos.y, worldPos.z);
	TriMesh::FVIter fv_it = modelMesh.fv_iter(fh);
	TriMesh::VertexHandle closestVH = *fv_it;
	TriMesh::Point v1 = modelMesh.point(*fv_it);
	++fv_it;

	minDistance = (p - v1).norm();
	for (; fv_it.is_valid(); ++fv_it)
	{
		TriMesh::Point v = modelMesh.point(*fv_it);
		double distance = (p - v).norm();
		if (minDistance > distance)
		{
			minDistance = distance;
			closestVH = *fv_it;
		}
	}

	return modelMesh.point(closestVH);
}

void Mesh::setPointPosition(glm::vec3 position)
{
	glBindBuffer(GL_ARRAY_BUFFER, selectedVertVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), glm::value_ptr(position), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

void Mesh::updateSelectedBufferObjects()
{
	selectedMesh.request_vertex_status();
	selectedMesh.request_face_status();
	selectedMesh.request_edge_status();
	selectedMesh.request_face_normals();
	selectedMesh.update_normals();

	MeshData::SelectedMeshData& lastSelectedMeshRef = (*(selectedMeshData.end() - 1));

	// Get vertices
	lastSelectedMeshRef.vertices.clear();
	lastSelectedMeshRef.texcoords.clear();
	for (TriMesh::VertexIter v_it = selectedMesh.vertices_begin(); v_it != selectedMesh.vertices_end(); ++v_it)
	{
		lastSelectedMeshRef.vertices.push_back(selectedMesh.point(*v_it));
		lastSelectedMeshRef.texcoords.push_back(glm::vec2(0.0f));
	}
		
	// Get indices
	lastSelectedMeshRef.indices.clear();
	for (TriMesh::FaceIter f_it = selectedMesh.faces_begin(); f_it != selectedMesh.faces_end(); ++f_it)
	{
		for (TriMesh::FaceVertexIter fv_it = selectedMesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
		{
			lastSelectedMeshRef.indices.push_back(fv_it->idx());
		}
	}

	glBindVertexArray(lastSelectedMeshRef.vao);

	glBindBuffer(GL_ARRAY_BUFFER, lastSelectedMeshRef.vbo[VertexObjType::POSITION]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriMesh::Point) * lastSelectedMeshRef.vertices.size(), lastSelectedMeshRef.vertices.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, lastSelectedMeshRef.vbo[VertexObjType::TEXCOORD]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * lastSelectedMeshRef.texcoords.size(), lastSelectedMeshRef.texcoords.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lastSelectedMeshRef.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * lastSelectedMeshRef.indices.size(), lastSelectedMeshRef.indices.data(), GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::updateSelectedFVMap(int face_3verticesID[])
{
	for (TriMesh::FaceIter f_it = selectedMesh.faces_begin(); f_it != selectedMesh.faces_end(); f_it++)
	{
		int original_corre_id = selectedMesh.property(faceIdPropHanlde, *f_it) - 1;

		auto faceID_it = selectedModelFaceMap.find(original_corre_id);

		if (faceID_it != selectedModelFaceMap.end())
			faceID_it->second = f_it->idx();
	}

	bool found_3vertices[3] = { false };

	for (auto v_it = selectedMesh.vertices_begin(); v_it != selectedMesh.vertices_end(); v_it++)
	{
		int original_corre_id = selectedMesh.property(vertIdPropHandle, *v_it) - 1;

		auto vertID_it = selectedModelVertMap.find(original_corre_id);

		if (vertID_it != selectedModelVertMap.end())
		{
			vertID_it->second = v_it->idx();

			if (found_3vertices[0] == false && original_corre_id == face_3verticesID[0])
				found_3vertices[0] = true;
			else if (found_3vertices[1] == false && original_corre_id == face_3verticesID[1])
				found_3vertices[1] = true;
			else if (found_3vertices[2] == false && original_corre_id == face_3verticesID[2])
				found_3vertices[2] = true;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (found_3vertices[i] == false)
		{
			selectedModelVertMap.erase(face_3verticesID[i]);
			printf("erase %d\n", face_3verticesID[i]);
		}
	}
}

void Mesh::initSelectedBufferObjs()
{
	MeshData::SelectedMeshData& data = (*(selectedMeshData.end() - 1));

	// VAO
	glGenVertexArrays(1, &data.vao);
	glBindVertexArray(data.vao);

	// VBO
	glGenBuffers(2, data.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, data.vbo[VertexObjType::POSITION]);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TriMesh::Point), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, data.vbo[VertexObjType::TEXCOORD]);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	
	// EBO
	glGenBuffers(1, &data.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.ebo);

	glBindVertexArray(0);
}

void Mesh::loadSelectedBufferObjs()
{
	MeshData::SelectedMeshData& data = (*(selectedMeshData.end() - 1));

	// VAO
	glGenVertexArrays(1, &data.vao);
	glBindVertexArray(data.vao);

	// VBO
	glGenBuffers(2, data.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, data.vbo[VertexObjType::POSITION]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriMesh::Point) * data.vertices.size(), data.vertices.data(), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TriMesh::Point), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, data.vbo[VertexObjType::TEXCOORD]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * data.texcoords.size(), data.texcoords.data(), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

	// EBO
	glGenBuffers(1, &data.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * data.indices.size(), data.indices.data(), GL_DYNAMIC_DRAW);

	glBindVertexArray(0);

}
