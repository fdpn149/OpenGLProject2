#include "pch.h"
#include "Mesh.h"
#include <GL/glew.h>
#include <vector>
#include "Shader.h"

Mesh::Mesh()
{
	modelMat = glm::mat4(1.0f);

	OpenMesh::IO::read_mesh(model, "assets/models/armadillo.obj");

	model.request_face_normals();
	model.request_vertex_status();
	model.request_face_status();
	model.request_edge_status();
	model.update_normals();

	std::vector<TriMesh::Point> vertices;
	vertices.reserve(model.n_vertices());
	for (TriMesh::VertexIter v_it = model.vertices_begin(); v_it != model.vertices_end(); ++v_it)
	{
		vertices.push_back(model.point(*v_it));

		TriMesh::Point p = model.point(*v_it);
	}

	std::vector<uint> indices;
	indices.reserve(model.n_faces() * 3);
	for (TriMesh::FaceIter f_it = model.faces_begin(); f_it != model.faces_end(); ++f_it)
	{
		for (TriMesh::FaceVertexIter fv_it = model.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
		{
			indices.push_back(fv_it->idx());
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriMesh::Point) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), &indices[0], GL_STATIC_DRAW);


	glGenBuffers(1, &vert_vbo);
}

void Mesh::draw()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, model.n_faces() * 3, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}

void Mesh::drawFace()
{
	glBindVertexArray(vao);

	for (uint face : selectedFace)
	{
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLuint*)(face * 3 * sizeof(GLuint)));
	}

	glBindVertexArray(0);
}

void Mesh::drawPoint()
{
	glPointSize(15.0f);
	glDrawArrays(GL_POINTS, 0, 1);
}

void Mesh::addSelectedFace(uint faceID)
{
	if (faceID >= 0 && faceID < model.n_faces())
	{
		selectedFace.insert(faceID);
	}
}

void Mesh::deleteSelectedFace(uint faceID)
{
	selectedFace.erase(faceID);
}

TriMesh::Point Mesh::findClosestPoint(uint faceID, glm::vec3 worldPos)
{
	OpenMesh::FaceHandle fh = model.face_handle(faceID);
	if (!fh.is_valid())
	{
		printf("invalid\n");
		return TriMesh::Point();
	}

	double minDistance = 0.0;
	TriMesh::Point p(worldPos.x, worldPos.y, worldPos.z);
	TriMesh::FVIter fv_it = model.fv_iter(fh);
	TriMesh::VertexHandle closestVH = *fv_it;
	TriMesh::Point v1 = model.point(*fv_it);
	++fv_it;

	minDistance = (p - v1).norm();
	for (; fv_it.is_valid(); ++fv_it)
	{
		TriMesh::Point v = model.point(*fv_it);
		double distance = (p - v).norm();
		if (minDistance > distance)
		{
			minDistance = distance;
			closestVH = *fv_it;
		}
	}

	return model.point(closestVH);
}

void Mesh::setPointPosition(glm::vec3 position)
{
	glBindBuffer(GL_ARRAY_BUFFER, vert_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), glm::value_ptr(position), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}
