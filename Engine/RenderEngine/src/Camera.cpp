#include "Camera.h"

Camera::Camera(float aspectRatio)
{
	m_camera.eye = glm::vec3(7.0);
	m_center = glm::vec3(0.0, 0.0, 0.0);
	m_up = glm::vec3(0.0, 1.0, 0.0);
	m_angles = glm::vec2(glm::pi<float>() / 3.0f, 0.0f); //theta , phi
	m_radius = 40.0f;
	m_fov = glm::radians(35.0f);
	m_aspectRatio = aspectRatio;
	m_near = 0.01f;
	m_camera.far = 1000.0f;
	m_camera.viewMatrix = glm::lookAt(m_camera.eye, m_center, m_up);
	m_camera.projMatrix = glm::perspective(m_fov, m_aspectRatio, m_near, m_camera.far);
}

void Camera::uploadCamera(ShaderSet* shaderSet)
{
	glUniform3fv(glGetUniformLocation(shaderSet->getProgramID(), "eye"), 1, glm::value_ptr(m_camera.eye));
	glUniformMatrix4fv(glGetUniformLocation(shaderSet->getProgramID(), "viewMatrix"), 1, GL_FALSE, glm::value_ptr(m_camera.viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shaderSet->getProgramID(), "projMatrix"), 1, GL_FALSE, glm::value_ptr(m_camera.projMatrix));
}

void Camera::updateProjMatrix()
{
	m_camera.projMatrix = glm::perspective(m_fov, m_aspectRatio, m_near, m_camera.far);
}

void Camera::updateViewMatrix()
{
	m_camera.viewMatrix = glm::lookAt(m_camera.eye, m_center, m_up);
}

void Camera::updateCamera()
{
	updateViewMatrix();
	updateProjMatrix();
}



