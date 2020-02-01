#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/ext.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "Instance.h"
#include "Scene.h"
class Renderer {
public:
	Renderer();

	void render(Scene* scene);
private:
	Instance* m_instance;
	GLFWwindow* m_window;
};