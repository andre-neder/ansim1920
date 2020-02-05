#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/ext.hpp"
#include <iostream>
#include <string>
#include <vector>

class Physics {
public:
	Physics();
	glm::vec3 getGravity();

protected:
	glm::vec3 m_gravity;
};