#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/ext.hpp"
#include "Settings.h"

class Instance {
public:
	static Instance* getInstance();
	Settings* m_settings;
	glm::vec2 m_cursorPos;
private:
	Instance();
	static Instance* m_instance;
};

