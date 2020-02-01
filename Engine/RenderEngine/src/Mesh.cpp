#include "Mesh.h"



Mesh::Mesh()
{
}

Mesh::Mesh(std::string file)
{
	loadOBJ(file);
	computeMinMax();
}

Mesh::Mesh(std::vector<glm::vec4> vertices, std::vector<unsigned int> indices)
{
	m_vertices = vertices;
	m_points = vertices.size();
	m_index = indices;
	m_indices = indices.size();
	createBuffers();
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &m_vertexbuffer);
	glDeleteBuffers(1, &m_normalbuffer);
	glDeleteBuffers(1, &m_uvbuffer);
	glDeleteBuffers(1, &m_tangentbuffer);
	glDeleteBuffers(1, &m_indexlist);
	glDeleteVertexArrays(1, &m_vao);
}

void Mesh::loadOBJ(std::string path)
{
	std::ifstream infile(path);
	std::string line;
	std::vector<char*> tmp;
	std::vector<glm::vec3> tmp_normals;
	std::vector<glm::vec4> tmp_points;
	std::vector<glm::vec2> tmp_uvs;
	std::vector<std::string> tmp_indices;
	std::vector<unsigned int> vertind;
	std::vector<unsigned int> uvind;
	std::vector<unsigned int> norm_mind;

	char* x;
	while (std::getline(infile, line)) {
		char str[256];
		strcpy(str, line.c_str());
		if (line.at(0)=='#') {
			continue;//comment
		}
		if (line.empty()) {
			continue;//empty
		}
		if (line.at(0) == 'o') {
			continue;//modelname
		}
		if (line.at(0) == 's') {
			continue;//glättung
		}
		if (line.at(0)=='v'&& line.at(1) == ' ') { //vertex
			x = strtok(str, " ");
			for (int i = 0; i < 3;i++) {
				x = strtok(NULL, " ");
				tmp.push_back(x);
			}
			tmp_points.push_back(glm::vec4(atof(tmp[0]), atof(tmp[1]), atof(tmp[2]), 1.0));
			tmp.clear();
		}
		if (line.at(0) == 'v'&& line.at(1) == 't') {//uv
			x = strtok(str, " ");
			for (int i = 0; i < 2; i++) {
				x = strtok(NULL, " ");
				tmp.push_back(x);
			}
			tmp_uvs.push_back(glm::vec2(atof(tmp[0]), atof(tmp[1])));
			tmp.clear();
		}
		if (line.at(0) == 'v'&& line.at(1) == 'n') {//normale
			x = strtok(str, " ");
			for (int i = 0; i < 3; i++) {
				x = strtok(NULL, " ");
				tmp.push_back(x);
			}
			tmp_normals.push_back(glm::vec3(atof(tmp[0]), atof(tmp[1]), atof(tmp[2])));
			tmp.clear();
		}
		if (line.at(0) == 'f') { //face
			x = strtok(str, " ");
			for (int i = 0; i < 3; i++) {
				x = strtok(NULL, " ");
				tmp.push_back(x);
			}
			tmp_indices.push_back(tmp[0]); //vertex1
			tmp_indices.push_back(tmp[1]); //vertex2
			tmp_indices.push_back(tmp[2]); //vertex3
			tmp.clear();
		}
	}
	char str[256];
	for (int i = 0; i < tmp_indices.size();i++) {
		strcpy(str, tmp_indices[i].c_str());
		x = strtok(str, "/");
		for (int j = 0; j < 2; j++) {
			tmp.push_back(x);
			x = strtok(NULL, "/");
		}
		tmp.push_back(x);
		vertind.push_back(atoi(tmp[0])-1);	//vertex index
		uvind.push_back(atoi(tmp[1])-1);	//uv index
		norm_mind.push_back(atoi(tmp[2])-1);	//normal index
		tmp.clear();
	}
	m_points = tmp_indices.size();				//number of vertices
	m_indices = tmp_indices.size();				//number of indices
	for (int i = 0; i < m_indices; i++) {
		m_vertices.push_back(tmp_points[vertind[i]]);
		m_normals.push_back(tmp_normals[norm_mind[i]]);
		m_uvs.push_back(tmp_uvs[uvind[i]]);
		m_index.push_back(i);
	}
	createBuffers();
}

void Mesh::createBuffers()
{	
	glGenBuffers(1, &m_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, m_points * sizeof(glm::vec4), &m_vertices[0], GL_STATIC_DRAW);
	if (m_uvs.empty() == false) {
		glGenBuffers(1, &m_uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, m_points * sizeof(glm::vec2), &m_uvs[0], GL_STATIC_DRAW);
	}
	if (m_normals.empty() == false) {
		glGenBuffers(1, &m_normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, m_points * sizeof(glm::vec3), &m_normals[0], GL_STATIC_DRAW);
	
		computeTangents();

		glGenBuffers(1, &m_tangentbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_tangentbuffer);
		glBufferData(GL_ARRAY_BUFFER, m_tangents.size() * sizeof(glm::vec3), &m_tangents[0], GL_STATIC_DRAW);
	
		glGenBuffers(1, &m_bitangentbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_bitangentbuffer);
		glBufferData(GL_ARRAY_BUFFER, m_bitangents.size() * sizeof(glm::vec3), &m_bitangents[0], GL_STATIC_DRAW);
	}
	glGenBuffers(1, &m_indexlist);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexlist);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices * sizeof(unsigned int), &m_index[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_vao);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	if (m_uvs.empty() == false) {
		glBindBuffer(GL_ARRAY_BUFFER, m_uvbuffer);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}
	if (m_normals.empty() == false) {
		glBindBuffer(GL_ARRAY_BUFFER, m_normalbuffer);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
		glBindBuffer(GL_ARRAY_BUFFER, m_tangentbuffer);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
		glBindBuffer(GL_ARRAY_BUFFER, m_bitangentbuffer);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexlist);

	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
}

glm::vec3 Mesh::getMin()
{
	return m_min;
}

glm::vec3 Mesh::getMax()
{
	return m_max;
}

int Mesh::getPoints()
{
	return m_points;
}

int Mesh::getIndices()
{
	return m_indices;
}

std::vector<glm::vec4> Mesh::getVertices()
{
	return m_vertices;
}

void Mesh::computeTangents()
{
	m_tangents.resize(m_vertices.size());

	for (int i = 0; i < m_indices; i += 3)
	{
		// Edges of the triangle : postion delta
		int index_v0 = m_index[i];
		int index_v1 = m_index[i + 1];
		int index_v2 = m_index[i + 2];

		glm::vec4 deltaPos1 = m_vertices[index_v1] - m_vertices[index_v0];
		glm::vec4 deltaPos2 = m_vertices[index_v2] - m_vertices[index_v0];
		/*
		glm::vec4 deltaPos1 = m_vertices[index_v0]-m_vertices[index_v1];
		glm::vec4 deltaPos2 = m_vertices[index_v2]-m_vertices[index_v1];
		*/
		// UV delta

		glm::vec2 deltaUV1 = m_uvs[index_v1] - m_uvs[index_v0];
		glm::vec2 deltaUV2 = m_uvs[index_v2] - m_uvs[index_v0];
		/*
		glm::vec2 deltaUV1 = m_uvs[index_v0]-m_uvs[index_v1];
		glm::vec2 deltaUV2 = m_uvs[index_v2]-m_uvs[index_v1];
		*/
		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec4 tmp = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
		glm::vec3 tangent = glm::vec3(tmp.x, tmp.y, tmp.z);

		// Set the same tangent for all three vertices of the triangle
		m_tangents[index_v0] = tangent;
		m_tangents[index_v1] = tangent;
		m_tangents[index_v2] = tangent;
	}
}

void Mesh::computeMinMax()
{
	m_min = glm::vec3(0.0);
	m_max = glm::vec3(0.0);
	for (int i = 0; i < m_vertices.size(); i++) {
		if (m_vertices[i].x < m_min.x)
			m_min.x = m_vertices[i].x;
		if (m_vertices[i].x > m_max.x)
			m_max.x = m_vertices[i].x;
		if (m_vertices[i].y < m_min.y)
			m_min.y = m_vertices[i].y;
		if (m_vertices[i].y > m_max.y)
			m_max.y = m_vertices[i].y;
		if (m_vertices[i].z < m_min.z)
			m_min.z = m_vertices[i].z;
		if (m_vertices[i].z > m_max.z)
			m_max.z = m_vertices[i].z;
	}

}



void Mesh::render()
{
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_indices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::render(GLenum mode)
{
	glBindVertexArray(m_vao);
	glDrawElements(mode, m_indices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::bind()
{
	glBindVertexArray(m_vao);
}

void Mesh::unbind()
{
	glBindVertexArray(0);
}
