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

Mesh::Mesh()
	: modelMat(glm::mat4(1.0f))
{
}

Mesh::Mesh(const std::string& file)
	: modelMat(glm::mat4(1.0f))
{
	load(file);
}

void Mesh::load(const std::string& file)
{
	if (OpenMesh::IO::read_mesh(mesh, file))
	{
		/* OpenMesh */

		mesh.request_vertex_status();
		mesh.request_edge_status();
		mesh.request_face_status();
		mesh.request_face_normals();
		mesh.update_normals();


		/* Get vertices */

		std::vector<TriMesh::Point> vertices;
		vertices.reserve(mesh.n_vertices());

		for (TriMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
		{
			vertices.push_back(mesh.point(*v_it));
		}


		/* Get indices */

		std::vector<unsigned int> indices;
		indices.reserve(mesh.n_faces() * 3);

		for (TriMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it)
		{
			for (TriMesh::FaceVertexIter fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
			{
				indices.push_back(fv_it->idx());
			}
		}


		/* VAO */
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);


		/* VBO */
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TriMesh::Point) * vertices.size(), vertices.data(), GL_STATIC_DRAW);


		/* EBO */
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);

		/* TEMP */
		glGenBuffers(1, &vert_vbo);

		glGenVertexArrays(1, &vao3);
		glGenBuffers(1, &vbo3);
	}
	else
	{
		std::cout << "Failed to load mesh." << std::endl;
	}
}

void Mesh::draw()
{
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TriMesh::Point), (void*)0);

	glDrawElements(GL_TRIANGLES, mesh.n_faces() * 3, GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
}

void Mesh::drawFaceByIds(std::set<unsigned int> faceIds)
{
	glBindVertexArray(vao);

	for (unsigned int id : faceIds)
	{
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (unsigned int*)(id * 3 * sizeof(unsigned int)));
	}

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

TriMesh::Point Mesh::findClosestPoint(uint faceID, glm::vec3 worldPos)
{
	OpenMesh::FaceHandle fh = mesh.face_handle(faceID);
	if (!fh.is_valid())
	{
		printf("invalid\n");
		return TriMesh::Point();
	}

	double minDistance = 0.0;
	TriMesh::Point p(worldPos.x, worldPos.y, worldPos.z);
	TriMesh::FVIter fv_it = mesh.fv_iter(fh);
	TriMesh::VertexHandle closestVH = *fv_it;
	TriMesh::Point v1 = mesh.point(*fv_it);
	++fv_it;

	minDistance = (p - v1).norm();
	for (; fv_it.is_valid(); ++fv_it)
	{
		TriMesh::Point v = mesh.point(*fv_it);
		double distance = (p - v).norm();
		if (minDistance > distance)
		{
			minDistance = distance;
			closestVH = *fv_it;
		}
	}

	return mesh.point(closestVH);
}

void Mesh::setPointPosition(glm::vec3 position)
{
	glBindBuffer(GL_ARRAY_BUFFER, vert_vbo);
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

std::vector<TriMesh::Point> Mesh::getFaceVerticesById(unsigned int faceId)
{
	std::vector<TriMesh::Point> faceVertices;

	if (faceId > 0 && faceId <= mesh.n_faces())
	{
		TriMesh::FaceHandle faceHandle = mesh.face_handle(faceId);
		
		for (TriMesh::FVIter fv_it = mesh.fv_begin(faceHandle); fv_it.is_valid(); ++fv_it)
		{
			faceVertices.push_back(mesh.point(*fv_it));
		}
	}

	return faceVertices;
}
