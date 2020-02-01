#include "Lightsource.h"


Lightsource::Lightsource()
{
	m_light.color = glm::vec4(1.0);
	m_light.position = glm::vec4(1.0);
	m_light.direction = -m_light.position;
	m_light.attenuation = glm::vec2(0.0, 1.0);
	m_light.cutoff = 0.0f;
	m_shadowFBO = new ShadowFBO(m_light.position.w);
	m_light.shadowMapID = m_shadowFBO->getHandleARB();
	m_light.lightMatrix = glm::mat4(1.0);
	m_light.farPlane = glm::sqrt(m_light.attenuation.x* m_light.attenuation.x+396* m_light.attenuation.y- m_light.attenuation.x)/2* m_light.attenuation.y;
	prepareShadowMap();
	createMesh();
}

Lightsource::~Lightsource()
{
}

Lightsource::Lightsource(glm::vec4 color, glm::vec4 position)
{
	m_light.color = color;
	m_light.position = position;
	m_light.direction = -m_light.position;
	m_light.attenuation = glm::vec2(0.0, 1.0);
	m_light.cutoff = 0.0f;
	m_shadowFBO = new ShadowFBO(m_light.position.w);
	m_light.shadowMapID = m_shadowFBO->getHandleARB();
	m_light.lightMatrix = glm::mat4(1.0);
	m_light.farPlane = 100.0f;
	if(m_light.position.w == 0)
		m_light.farPlane = 20.0f;
	prepareShadowMap();
	createMesh();
}

Lightsource::Lightsource(glm::vec4 color, glm::vec4 position, float cutoff)
{
	m_light.color = color;
	m_light.position = position;
	m_light.direction = -m_light.position;
	m_light.attenuation = glm::vec2(0.0, 1.0);
	m_light.cutoff = cutoff;
	m_shadowFBO = new ShadowFBO(m_light.position.w);
	m_light.shadowMapID = m_shadowFBO->getHandleARB();
	m_light.lightMatrix = glm::mat4(1.0);
	m_light.farPlane = 100.0f;
	if (m_light.position.w == 0)
		m_light.farPlane = 20.0f;
	prepareShadowMap();
	createMesh();
}

Lightsource::Lightsource(glm::vec4 color, glm::vec4 position, glm::vec2 attenuation)
{
	m_light.color = color;
	m_light.position = position;
	m_light.direction = -m_light.position;
	m_light.attenuation = attenuation;
	m_light.cutoff = 0.0f;
	m_shadowFBO = new ShadowFBO(m_light.position.w);
	m_light.shadowMapID = m_shadowFBO->getHandleARB();
	m_light.lightMatrix = glm::mat4(1.0);
	m_light.farPlane = 100.0f;
	if (m_light.position.w == 0)
		m_light.farPlane = 20.0f;
	prepareShadowMap();
	createMesh();
}

LightStruct Lightsource::getLightStruct()
{
	return m_light;
}

void Lightsource::setPosition(glm::vec3 position)
{
	m_light.position = glm::vec4(position, m_light.position.w);
	m_light.direction = -m_light.position;
}

void Lightsource::bindShadowFBO(ShaderSet* shaderSet)
{
	m_shadowFBO->bindForWriting();
	glUniform1f(glGetUniformLocation(shaderSet->getProgramID(), "farPlane"),m_light.farPlane);
	glUniformMatrix4fv(glGetUniformLocation(shaderSet->getProgramID(), "lightMatrix"), 1, GL_FALSE, glm::value_ptr(m_light.lightMatrix));
	glCullFace(GL_FRONT);
}

void Lightsource::bindShadowFBO(ShaderSet* shaderSet, unsigned int face)
{
	m_shadowFBO->bindForWriting(face);
	glUniform1f(glGetUniformLocation(shaderSet->getProgramID(), "farPlane"), m_light.farPlane);
	glUniform4fv(glGetUniformLocation(shaderSet->getProgramID(), "lightPos"), 1, glm::value_ptr(m_light.position));
	glUniformMatrix4fv(glGetUniformLocation(shaderSet->getProgramID(), "lightMatrix"), 1, GL_FALSE, glm::value_ptr(m_shadowTransforms[face]));
	glCullFace(GL_FRONT);
}

void Lightsource::unbindShadowFBO()
{
	glCullFace(GL_BACK);
	m_shadowFBO->unbind();
}

void Lightsource::updateLightMatrix()
{
	prepareShadowMap();
	m_modelMatrix = glm::translate(glm::scale(glm::mat4(1.0), glm::vec3(0.25f)), glm::vec3(4.0)*glm::vec3(m_light.position));
}

void Lightsource::renderMesh(ShaderSet* shaderSet)
{
	glUniform4fv(glGetUniformLocation(shaderSet->getProgramID(), "color"), 1, glm::value_ptr(m_light.color));
	glUniformMatrix4fv(glGetUniformLocation(shaderSet->getProgramID(), "modelMatrix"), 1, GL_FALSE, glm::value_ptr(m_modelMatrix));
	m_mesh->render();
}

void Lightsource::prepareShadowMap()
{
	if (m_light.position.w == 1.0f) {
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, m_light.farPlane);
		m_shadowTransforms.clear();
		m_shadowTransforms.push_back(shadowProj *
			glm::lookAt(glm::vec3(m_light.position), glm::vec3(m_light.position) + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		m_shadowTransforms.push_back(shadowProj *
			glm::lookAt(glm::vec3(m_light.position), glm::vec3(m_light.position) + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		m_shadowTransforms.push_back(shadowProj *
			glm::lookAt(glm::vec3(m_light.position), glm::vec3(m_light.position) + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
		m_shadowTransforms.push_back(shadowProj *
			glm::lookAt(glm::vec3(m_light.position), glm::vec3(m_light.position) + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
		m_shadowTransforms.push_back(shadowProj *
			glm::lookAt(glm::vec3(m_light.position), glm::vec3(m_light.position) + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
		m_shadowTransforms.push_back(shadowProj *
			glm::lookAt(glm::vec3(m_light.position), glm::vec3(m_light.position) + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
	}
	else if (m_light.position.w == 0.0f && m_light.cutoff == 0.0f) {
		glm::mat4 shadowProj = glm::ortho(-10.0f,10.0f, -10.0f, 10.0f, 0.1f, m_light.farPlane);
		glm::vec3 view = glm::vec3(m_light.position) + m_light.direction;
		glm::vec3 up = glm::vec3(0.0, -1.0, 0.0);
		if (view.x == 0 && view.z == 0)
			up = glm::vec3(-1.0, 0.0, 0.0);
		glm::mat4 shadowView = glm::lookAt(glm::vec3(m_light.position), view, up);
		m_light.lightMatrix = shadowProj * shadowView;
	}
	else if (m_light.position.w == 0.0f && m_light.cutoff != 0.0f) {
		glm::mat4 shadowProj = glm::perspective(m_light.cutoff, 1.0f, 0.1f, m_light.farPlane);
		glm::vec3 view = glm::vec3(m_light.position) + m_light.direction;
		glm::vec3 up = glm::vec3(0.0, -1.0, 0.0);
		if (view.x == 0 && view.z == 0)
			up = glm::vec3(-1.0, 0.0, 0.0);
		glm::mat4 shadowView = glm::lookAt(glm::vec3(m_light.position), view, up);
		m_light.lightMatrix = shadowProj * shadowView;
	}
}

void Lightsource::createMesh()
{
	m_mesh = new Mesh(MODEL_PATH"cube.obj");
	m_modelMatrix = glm::translate(glm::scale(glm::mat4(1.0),glm::vec3(0.25f)), glm::vec3(m_light.position));
}





