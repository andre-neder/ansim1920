#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/ext.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "ShaderSet.h"

struct CameraStruct{
	glm::mat4 projMatrix;
	glm::mat4 viewMatrix;
	glm::vec3 eye;
	float far;
};

class Camera {
public:
	Camera(float aspectRatio);

	void uploadCamera(ShaderSet* shaderSet);
	void updateProjMatrix();
	void updateViewMatrix();
	void updateCamera();
//private:
	glm::vec3 m_center;
	glm::vec3 m_up;
	glm::vec2 m_angles;
	float m_radius;
	float m_fov;
	float m_aspectRatio;
	float m_near;
	CameraStruct m_camera;

private:
	GLuint m_cameraBufferID;
};