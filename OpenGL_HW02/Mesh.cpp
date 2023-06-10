#include "pch.h"
#include "Mesh.h"
#include <GL/glew.h>
#include <vector>
#include <eigen3/Eigen/Dense>
#include "Shader.h"

Mesh::Mesh()
{
	modelMat = glm::mat4(1.0f);

	bool status;
	status = OpenMesh::IO::read_mesh(model, "assets/models/UnionSphere.obj");
	//status = OpenMesh::IO::read_mesh(model, "assets/models/armadillo.obj");
	//status = OpenMesh::IO::read_mesh(model, "assets/models/icosahedron.obj");
	//status = OpenMesh::IO::read_mesh(model, "assets/models/octahedron.obj");
	//status = OpenMesh::IO::read_mesh(model, "assets/models/rhombic-triacontahedron.obj");

	if (status == false)
		throw "Error";

	model.request_vertex_status();
	model.request_face_status();
	model.request_edge_status();
	model.request_face_normals();
	model.update_normals();

	std::vector<TriMesh::Point> vertices;

	for (TriMesh::VertexIter v_it = model.vertices_begin(); v_it != model.vertices_end(); ++v_it)
		vertices.push_back(model.point(*v_it));

	std::vector<uint> indices;
	for (TriMesh::FaceIter f_it = model.faces_begin(); f_it != model.faces_end(); ++f_it)
		for (TriMesh::FaceVertexIter fv_it = model.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
			indices.push_back(fv_it->idx());

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

	glBindVertexArray(0);

	glGenBuffers(1, &vert_vbo);


	glGenVertexArrays(1, &vao2);
	glGenBuffers(1, &vbo2);

	glGenVertexArrays(1, &vao3);
	glGenBuffers(1, &vbo3);


	selected.add_property(faceID_about_model);
	selected.add_property(vertexID_about_model);
}

glm::vec3 Mesh::pointToVec3(const TriMesh::Point& point)
{
	return glm::vec3(point[0], point[1], point[2]);
}

TriMesh::Point Mesh::percentToXY(float percent)
{
	if (percent >= 0.0f && percent < 0.25f)
	{
		return TriMesh::Point(percent * 4.0f, 0.0f, 0);
	}
	else if (percent >= 0.25f && percent < 0.5f)
	{
		return TriMesh::Point(1.0f, (percent - 0.25f) * 4.0f, 0);
	}
	else if (percent >= 0.5f && percent < 0.75f)
	{
		return TriMesh::Point(1.0f - (percent - 0.5f) * 4.0f, 1.0f, 0);
	}
	else if (percent >= 0.75f)
	{
		return TriMesh::Point(0.0f, 1.0f - (percent - 0.75f) * 4.0f, 0);
	}
	return TriMesh::Point(-1.0f);
}

std::vector<TriMesh::VertexHandle> Mesh::addVerticesToSelected(const TriMesh::FaceHandle& fh_model)
{
	std::vector<TriMesh::VertexHandle> vh_in_selected;

	for (auto fv_it = model.fv_begin(fh_model); fv_it.is_valid(); fv_it++)
	{
		int id_to_find = fv_it->idx();
		TriMesh::VertexHandle vh;

		if (vertex_model_selected.find(id_to_find) != vertex_model_selected.end())
		{
			vh = selected.vertex_handle(vertex_model_selected[id_to_find]);
		}
		else
		{
			TriMesh::Point new_point = model.point(*fv_it);
			vh = selected.add_vertex(new_point);

			selected.property(vertexID_about_model, vh) = id_to_find + 1;

			vertex_model_selected[fv_it->idx()] = vh.idx();
		}

		vh_in_selected.push_back(vh);
	}

	return vh_in_selected;
}

void Mesh::updateBuffer()
{
	selected.request_vertex_status();
	selected.request_face_status();
	selected.request_edge_status();
	selected.request_face_normals();
	selected.update_normals();

	std::vector<TriMesh::Point> vertices;
	for (TriMesh::VertexIter v_it = selected.vertices_begin(); v_it != selected.vertices_end(); ++v_it)
		vertices.push_back(selected.point(*v_it));

	std::vector<uint> indices;
	for (TriMesh::FaceIter f_it = selected.faces_begin(); f_it != selected.faces_end(); ++f_it)
		for (TriMesh::FaceVertexIter fv_it = selected.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
			indices.push_back(fv_it->idx());

	glBindVertexArray(vao2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriMesh::Point) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::updateFVSelectedMap(int face_3verticesID[])
{
	for (TriMesh::FaceIter f_it = selected.faces_begin(); f_it != selected.faces_end(); f_it++)
	{
		int original_corre_id = selected.property(faceID_about_model, *f_it) - 1;

		auto faceID_it = face_model_selected.find(original_corre_id);

		if (faceID_it != face_model_selected.end())
			faceID_it->second = f_it->idx();
	}

	bool found_3vertices[3] = { false };

	for (auto v_it = selected.vertices_begin(); v_it != selected.vertices_end(); v_it++)
	{
		int original_corre_id = selected.property(vertexID_about_model, *v_it) - 1;

		auto vertID_it = vertex_model_selected.find(original_corre_id);

		if (vertID_it != vertex_model_selected.end())
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
			vertex_model_selected.erase(face_3verticesID[i]);
			printf("erase %d\n", face_3verticesID[i]);
		}
	}

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

	/*for (uint face : selectedFace)
	{
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLuint*)(face * 3 * sizeof(GLuint)));
	}*/

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

void Mesh::drawSelected()
{
	glBindVertexArray(vao2);
	glDrawElements(GL_TRIANGLES, selected.n_faces() * 3, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}

void Mesh::addSelectedFace(int faceID)
{
	if (faceID >= 0 && faceID < model.n_faces())
	{
		TriMesh::FaceHandle fh = model.face_handle(faceID);

		if (!model.status(fh).selected())
		{
			model.status(fh).set_selected(true);

			std::vector<TriMesh::VertexHandle> new_vertices = addVerticesToSelected(fh);

			TriMesh::FaceHandle new_fh = selected.add_face(new_vertices);

			selected.property(faceID_about_model, new_fh) = faceID + 1;

			face_model_selected[faceID] = new_fh.idx();

			updateBuffer();


#ifdef WRITE_OBJ
			OpenMesh::IO::write_mesh(selected, "assets/models/selected.obj");
#endif // WRITE_OBJ

		}
	}
}

void Mesh::deleteSelectedFace(int faceID)
{
	TriMesh::FaceHandle fh_in_model = model.face_handle(faceID);

	if (!fh_in_model.is_valid() || !model.status(fh_in_model).selected())
		return;

	model.status(fh_in_model).set_selected(false);
	int id = face_model_selected[faceID];

	TriMesh::FaceHandle fh_in_selected = selected.face_handle(id);

	if (!fh_in_selected.is_valid())
		fprintf(stderr, "Error\n");

	int face_3verticesID[3] = { 0 };

	int index = 0;
	for (auto fv_it = selected.fv_begin(fh_in_selected); fv_it.is_valid(); fv_it++)
	{
		face_3verticesID[index] = selected.property(vertexID_about_model, *fv_it) - 1;
		index++;
	}

	printf("\n");

	selected.delete_face(fh_in_selected);

	face_model_selected.erase(faceID);

	selected.garbage_collection();

	updateFVSelectedMap(face_3verticesID);

	updateBuffer();

#ifdef WRITE_OBJ
	OpenMesh::IO::write_mesh(selected, "assets/models/selected.obj");
#endif // WRITE_OBJ
}

TriMesh::Point Mesh::findClosestPoint(int faceID, glm::vec3 worldPos)
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

void Mesh::setLinePosition()
{
	glBindVertexArray(vao3);
	glBindBuffer(GL_ARRAY_BUFFER, vbo3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * lines.size() * 3, lines.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

void Mesh::calculateSurround(std::vector<TriMesh::Point>& points)
{
	points.clear();
	lines.clear();
	boundary_point3D_2D.clear();


	//find halfedges until it is on boundary
	TriMesh::HalfedgeIter h_it = selected.halfedges_begin();
	if (h_it == selected.halfedges_end())
		return;
	for (; h_it != selected.halfedges_end(); h_it++)
	{
		if (selected.is_boundary(*h_it))	//the halfedge is on boundary
			break;
	}

	TriMesh::HalfedgeHandle hh_init(*h_it);	//first halfedge on boundary

	TriMesh::HalfedgeHandle hh(hh_init);

	std::vector<TriMesh::Point> to_point;
	std::vector<float> percent;

	//travel all halfedge on boundary
	float total = 0;
	int t = 0;
	do
	{
		TriMesh::VertexHandle vh = selected.to_vertex_handle(hh);
		TriMesh::VertexHandle vh2 = selected.from_vertex_handle(hh);

		TriMesh::Point p1 = selected.point(vh);
		TriMesh::Point p2 = selected.point(vh2);

		to_point.push_back(p1);

		float length = (p1 - p2).norm();

		percent.push_back(length);
		total += length;

		hh = selected.next_halfedge_handle(hh);
		if (!hh.is_valid())
			return;
	} while (hh != hh_init);



	//calculate percent
	float sum = 0;
	for (int i = 0; i < percent.size(); i++)
	{
		sum += percent[i] / total;
		TriMesh::Point point = percentToXY(sum);	//transform the percent value to 2d xy coordinate value
		points.push_back(point);
		boundary_point3D_2D[to_point[i]] = point;
	}
}



void Mesh::calculateInside(std::vector<std::vector<TriMesh::Point>>& points)
{
	points.clear();

	//int count = 0;
	//std::vector<TriMesh::Point> outside_points_sum;
	//std::vector<std::vector<float>> inside_points_w;
	//std::map<TriMesh::Point, int> inside_points_w_index;

	//int insides_size = 0;

	////std::vector<std::vector<TriMesh::Point>> connect_point_group;
	//std::map<int, std::vector<TriMesh::Point>> connect_point_group;


	//for (auto v_it = selected.vertices_begin(); v_it != selected.vertices_end(); v_it++) //find center_point
	//{
	//	if (!selected.is_boundary(*v_it))	//is not boundary point
	//	{
	//		TriMesh::Point center_point = selected.point(*v_it);
	//		glm::vec3 center_vec = pointToVec3(center_point);

	//		TriMesh::Point prev_point, curr_point, next_point;
	//		glm::vec3 prev_vec, curr_vec, next_vec;
	//		glm::vec3 to_center, to_current;	//from prev/next to center/current

	//		TriMesh::Point total_outside(0.0f);
	//		float total_w = 0.0f;

	//		std::vector<float> insides;
	//		insides.resize(insides_size);

	//		std::vector<TriMesh::Point> connect;

	//		for (auto voh_it = selected.voh_begin(*v_it); voh_it != selected.voh_end(*v_it); voh_it++)
	//		{
	//			TriMesh::Point v = selected.point(selected.to_vertex_handle(*voh_it));
	//			if (boundary_point3D_2D.find(v) != boundary_point3D_2D.end()) //boundary point
	//			{
	//				TriMesh::Point p = boundary_point3D_2D[v];
	//				connect.push_back(p);
	//				prev_point = selected.point(selected.opposite_vh(*voh_it));
	//				curr_point = selected.point(selected.to_vertex_handle(*voh_it));
	//				next_point = selected.point(selected.opposite_he_opposite_vh(*voh_it));

	//				prev_vec = pointToVec3(prev_point);
	//				curr_vec = pointToVec3(curr_point);
	//				next_vec = pointToVec3(next_point);

	//				to_center = center_vec - prev_vec;
	//				to_current = curr_vec - prev_vec;
	//				float gamma = glm::acos(glm::dot(to_center, to_current) / (to_center.length() * to_current.length()));

	//				to_center = center_vec - next_vec;
	//				to_current = curr_vec - next_vec;
	//				float beta = glm::acos(glm::dot(to_center, to_current) / (to_center.length() * to_current.length()));

	//				float w = 1.0f / glm::tan(beta) + 1.0f / glm::tan(gamma);
	//				total_w += w;
	//				total_outside += w * p;
	//			}
	//			else //inside point
	//			{
	//				prev_point = selected.point(selected.opposite_vh(*voh_it));
	//				curr_point = selected.point(selected.to_vertex_handle(*voh_it));
	//				next_point = selected.point(selected.opposite_he_opposite_vh(*voh_it));

	//				prev_vec = pointToVec3(prev_point);
	//				curr_vec = pointToVec3(curr_point);
	//				next_vec = pointToVec3(next_point);

	//				to_center = center_vec - prev_vec;
	//				to_current = curr_vec - prev_vec;
	//				float gamma = glm::acos(glm::dot(to_center, to_current) / (to_center.length() * to_current.length()));

	//				to_center = center_vec - next_vec;
	//				to_current = curr_vec - next_vec;
	//				float beta = glm::acos(glm::dot(to_center, to_current) / (to_center.length() * to_current.length()));

	//				float w = 1.0f / glm::tan(beta) + 1.0f / glm::tan(gamma);
	//				total_w += w;

	//				if (inside_points_w_index.find(v) != inside_points_w_index.end())
	//				{
	//					insides[inside_points_w_index[v]] = -w;
	//				}
	//				else
	//				{
	//					inside_points_w_index[v] = insides_size++;
	//					insides.push_back(-w);
	//				}

	//				connect.push_back(TriMesh::Point(inside_points_w_index[v]));	//create dummy point (make a mark)
	//			}
	//		}

	//		outside_points_sum.push_back(total_outside);

	//		if (inside_points_w_index.find(center_point) != inside_points_w_index.end())
	//		{
	//			insides[inside_points_w_index[center_point]] = total_w;
	//		}
	//		else
	//		{
	//			inside_points_w_index[center_point] = insides_size++;
	//			insides.push_back(total_w);
	//		}
	//		inside_points_w.push_back(insides);


	//		if (!connect.empty())
	//			connect_point_group[inside_points_w_index[center_point]] = connect;
	//	}
	//}

	//if (insides_size > 0)
	//{
	//	Eigen::MatrixXf A(insides_size, insides_size);
	//	Eigen::VectorXf b(insides_size);
	//	Eigen::VectorXf x;

	//	std::vector<float> all_x;

	//	inside_points_w.resize(insides_size);

	//	for (int k = 0; k < 2; k++)
	//	{
	//		for (int i = 0; i < insides_size; i++)
	//		{
	//			if (inside_points_w[i].size() < insides_size)
	//				inside_points_w[i].resize(insides_size);
	//			for (int j = 0; j < insides_size; j++)
	//			{
	//				A(j, i) = inside_points_w[i][j];
	//			}
	//			b(i) = outside_points_sum[i][k];
	//		}

	//		x = A.lu().solve(b);
	//		if (k == 0)
	//			all_x = std::vector<float>(x.data(), x.data() + x.size());

	//		//std::cout << (k == 0 ? 'x' : 'y') << " = " << x.transpose() << std::endl;
	//	}

	//	points.resize(insides_size);
	//	for (int i = 0; i < insides_size; i++)
	//	{
	//		for (int j = 0; j < connect_point_group[i].size(); j++)
	//		{
	//			if (connect_point_group[i][j][2] != 0)	//if z != 0 -> it is marked
	//				points[i].push_back(TriMesh::Point(all_x[connect_point_group[i][j][2]], x(connect_point_group[i][j][2]), 0.0f));
	//			else
	//				points[i].push_back(connect_point_group[i][j]);
	//			points[i].push_back(TriMesh::Point(all_x[i], x(i), 0));
	//		}
	//	}
	//}
}
