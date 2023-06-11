#include "pch.h"
#include "Mesh.h"

/*                 Standard                 */
#include <iostream>
#include <set>
#include <map>


/*                 OpenGL                   */
#include <GL/glew.h>


/*                 GLM                      */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


/*                 Eigen                    */
#include <eigen3/Eigen/Dense>


/*                 My Class                 */
#include "Shader.h"
#include "utilities.h"
#include "Config.h"
#include "ConvexCombMap.h"


Mesh::Mesh()
	: modelMat(glm::mat4(1.0f))
{
	selectedMesh.add_property(faceIdPropHanlde);
	selectedMesh.add_property(vertIdPropHandle);

	selectedTexId = Utils::loadTexture(Config::TEXTURE_PATH + "container.jpg");
}

Mesh::Mesh(const std::string& file)
{
	Mesh();
	load(file);
}

void Mesh::load(const std::string& file)
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

		std::vector<TriMesh::Point> vertices;
		vertices.reserve(modelMesh.n_vertices());

		for (TriMesh::VertexIter v_it = modelMesh.vertices_begin(); v_it != modelMesh.vertices_end(); ++v_it)
		{
			vertices.push_back(modelMesh.point(*v_it));
		}


		/* Get indices */

		std::vector<unsigned int> indices;
		indices.reserve(modelMesh.n_faces() * 3);

		for (TriMesh::FaceIter f_it = modelMesh.faces_begin(); f_it != modelMesh.faces_end(); ++f_it)
		{
			for (TriMesh::FaceVertexIter fv_it = modelMesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
			{
				indices.push_back(fv_it->idx());
			}
		}


		/* Initialize Model Buffer Objects */

		// VBO
		glGenBuffers(1, &modelVbo);
		glBindBuffer(GL_ARRAY_BUFFER, modelVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TriMesh::Point) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

		// VAO
		glGenVertexArrays(1, &modelVao);
		glBindVertexArray(modelVao);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TriMesh::Point), (void*)0);


		// EBO
		glGenBuffers(1, &modelEbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelEbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);


		/* Initialize Selected Buffer Object */

		// VAO
		glGenVertexArrays(1, &selectedVao);
		glBindVertexArray(selectedVao);

		// VBO
		glGenBuffers(1, &selectedVbo);
		glBindBuffer(GL_ARRAY_BUFFER, selectedVbo);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TriMesh::Point), 0);


		glGenBuffers(1, &selectedTexVbo);
		glBindBuffer(GL_ARRAY_BUFFER, selectedTexVbo);
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

		// EBO
		glGenBuffers(1, &selectedEbo);


		/* TEMP */
		glGenVertexArrays(1, &vao3);
		glGenBuffers(1, &vbo3);

		glBindVertexArray(0);
	}
	else
	{
		std::cout << "Failed to load mesh." << std::endl;
	}
}

void Mesh::draw()
{
	glBindVertexArray(modelVao);
	glDrawElements(GL_TRIANGLES, modelMesh.n_faces() * 3, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}

void Mesh::drawSelecetedFaces()
{
	for (auto texcoord : selectedTexcoords)
	{
		std::cout << "x: " << texcoord.x << " y: " << texcoord.y << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, selectedTexId);

	glBindVertexArray(selectedVao);
	glDrawElements(GL_TRIANGLES, selectedIndices.size(), GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}

void Mesh::drawPoint()
{
	glPointSize(15.0f);
	glDrawArrays(GL_POINTS, 0, 1);
}

void Mesh::drawLine()
{
	glBindVertexArray(vao3);
	glDrawArrays(GL_LINES, 0, lines.size());
	glBindVertexArray(0);
}

void Mesh::calcTexcoord()
{
	selectedTexcoords.resize(selectedVertices.size());

	for (int iii = 0; iii != selectedVertices.size(); ++iii)
	{
		selectedTexcoords[iii] = ConvexCombMap::map(Utils::toGlmVec3(selectedVertices[iii]));
	}

	glBindBuffer(GL_ARRAY_BUFFER, selectedTexVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * selectedTexcoords.size(), selectedTexcoords.data(), GL_STATIC_DRAW);
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

void Mesh::setLinePosition()
{
	glBindVertexArray(vao3);
	glBindBuffer(GL_ARRAY_BUFFER, vbo3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * lines.size() * 3, lines.data(), GL_STATIC_DRAW);
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

	// Get vertices
	selectedVertices.clear();
	for (TriMesh::VertexIter v_it = selectedMesh.vertices_begin(); v_it != selectedMesh.vertices_end(); ++v_it)
	{
		selectedVertices.push_back(selectedMesh.point(*v_it));
	}
		
	// Get indices
	selectedIndices.clear();
	for (TriMesh::FaceIter f_it = selectedMesh.faces_begin(); f_it != selectedMesh.faces_end(); ++f_it)
	{
		for (TriMesh::FaceVertexIter fv_it = selectedMesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
		{
			selectedIndices.push_back(fv_it->idx());
		}
	}
	
	glBindVertexArray(selectedVao);

	glBindBuffer(GL_ARRAY_BUFFER, selectedVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriMesh::Point) * selectedVertices.size(), selectedVertices.data(), GL_STATIC_DRAW);

	selectedTexcoords.resize(selectedVertices.size());
	glBindBuffer(GL_ARRAY_BUFFER, selectedTexVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * selectedTexcoords.size(), selectedTexcoords.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, selectedEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * selectedIndices.size(), selectedIndices.data(), GL_STATIC_DRAW);

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

