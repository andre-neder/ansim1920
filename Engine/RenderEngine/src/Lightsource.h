#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/ext.hpp"
#include "Mesh.h"
#include "ShaderSet.h"
#include "Texture.h"
#include "ShadowFBO.h"
#include "Defines.h"
#include <iostream>
#include <vector>
#define UP glm::vec3(0.0, 1.0, 0.0)

struct LightStruct {
	glm::vec4 color;
	glm::vec4 position;
	glm::vec3 direction;
	float cutoff;
	glm::vec2 attenuation;
	GLuint64 shadowMapID;
	glm::mat4 lightMatrix;
	float farPlane, pad0, pad1, pad2;
};

class Lightsource {
public:

	Lightsource();
	~Lightsource();
	Lightsource(glm::vec4 color, glm::vec4 position);
	Lightsource(glm::vec4 color, glm::vec4 position, float cutoff);
	Lightsource(glm::vec4 color, glm::vec4 position, glm::vec2 attenuation);
	LightStruct getLightStruct();
	void setPosition(glm::vec3 position);
	void bindShadowFBO(ShaderSet* shaderSet);
	void bindShadowFBO(ShaderSet* shaderSet, unsigned int face);
	void unbindShadowFBO();
	void updateLightMatrix();
	void renderMesh(ShaderSet* shaderSet);
	LightStruct m_light;
private:
	void prepareShadowMap();
	void createMesh();
	Mesh* m_mesh;
	glm::mat4 m_modelMatrix; // only for Mesh;
	std::vector<glm::mat4> m_shadowTransforms;
	ShadowFBO* m_shadowFBO;

};