#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/ext.hpp"

class SSBO {
public:
	SSBO(GLsizeiptr size,GLvoid* data);
private:
	GLuint m_bufferID;
};