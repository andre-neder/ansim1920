#ifndef MESH_H
#define MESH_H
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/ext.hpp"

class Mesh
{
public:
	Mesh();
	Mesh(std::string file);
	Mesh(std::vector<glm::vec4> vertices, std::vector<unsigned int> indices);
	~Mesh();
	void loadOBJ(std::string path);
	void render();
	void render(GLenum mode);
	void bind();
	void unbind();
	void createBuffers();
	glm::vec3 getMin();
	glm::vec3 getMax();
	int getPoints();
	int getIndices();
	std::vector<glm::vec4> getVertices();

private:
	GLuint m_vao;
	GLuint m_vertexbuffer;
	GLuint m_normalbuffer;
	GLuint m_uvbuffer;
	GLuint m_tangentbuffer;
	GLuint m_bitangentbuffer;
	GLuint m_indexlist;

	int m_points;
	int m_indices;

	std::vector<glm::vec4> m_vertices;
	std::vector<glm::vec3> m_normals;
	std::vector<glm::vec2> m_uvs;
	std::vector<glm::vec3> m_tangents;
	std::vector<glm::vec3> m_bitangents;
	std::vector<unsigned int> m_index;
	glm::vec3 m_min , m_max ;

	void computeTangents();
	void computeMinMax();
};

#endif