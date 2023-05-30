#include "pch.h"
#include "Mesh.h"
#include <GL/glew.h>
#include <vector>
#include "Shader.h"

Mesh::Mesh()
{
	modelMat = glm::mat4(1.0f);

	OpenMesh::IO::read_mesh(*this, "assets/models/UnionSphere.obj");

	this->request_face_normals();
	this->request_vertex_status();
	this->request_face_status();
	this->request_edge_status();
	this->update_normals();

	std::vector<Point> vertices;
	vertices.reserve(this->n_vertices());
	for (VertexIter v_it = this->vertices_begin(); v_it != this->vertices_end(); ++v_it)
	{
		vertices.push_back(this->point(*v_it));

		Point p = this->point(*v_it);
	}

	std::vector<uint> indices;
	indices.reserve(this->n_faces() * 3);
	for (FaceIter f_it = this->faces_begin(); f_it != this->faces_end(); ++f_it)
	{
		for (FaceVertexIter fv_it = this->fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
		{
			indices.push_back(fv_it->idx());
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
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
	glDrawElements(GL_TRIANGLES, this->n_faces() * 3, GL_UNSIGNED_INT, (void*)0);
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
	if (faceID >= 0 && faceID < this->n_faces())
	{
		selectedFace.insert(faceID);
	}
}

void Mesh::deleteSelectedFace(uint faceID)
{
	selectedFace.erase(faceID);
}

void Mesh::setPointPosition(glm::vec3 position)
{
	glBindBuffer(GL_ARRAY_BUFFER, vert_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), glm::value_ptr(position), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}
