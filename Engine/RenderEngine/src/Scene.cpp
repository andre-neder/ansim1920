#include "Scene.h"

Scene::Scene()
{
	m_vertexCount = 0;
	m_triangleCount = 0;
	m_instance = m_instance->getInstance();
}

Scene::~Scene()
{
	for (auto& l : m_lights) {
		l.second->~Lightsource();
	}
	for (auto& o : m_objects) {
		o.second->~Object();
	}
}

void Scene::render(ShaderSet* shaderSet)
{
	shaderSet->UseProgram();
	//glEnable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUniform1ui(glGetUniformLocation(shaderSet->getProgramID(), "renderShadows"), m_instance->m_settings->m_renderShadows);
	m_camera->uploadCamera(shaderSet);
	m_camera->updateCamera();
	updateLightBuffer();
	glViewport(0, 0, m_instance->m_settings->m_window_width, m_instance->m_settings->m_window_height);
	for (auto& x : m_objects) {
		x.second->render(shaderSet);
	}
}

void Scene::renderShadows(ShaderSet* shaderSet)
{
	shaderSet->UseProgram();
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (auto& l :  m_lights) {
		if (l.second->getLightStruct().position.w == 0.0) {
			l.second->bindShadowFBO(shaderSet);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			for (auto& x : m_objects) {
				x.second->renderMesh(shaderSet);
			}
			l.second->unbindShadowFBO();
		}
		else if (l.second->getLightStruct().position.w == 1.0) {
			l.second->bindShadowFBO(shaderSet);
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			for (unsigned int i = 0; i < 6; i++) {
				l.second->bindShadowFBO(shaderSet,i);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				for (auto& x : m_objects) {
					x.second->renderMesh(shaderSet);
				}
			}
			l.second->unbindShadowFBO();
		}
	}
}

void Scene::renderBoundingVolumes(ShaderSet* shaderSet)
{
	shaderSet->UseProgram();
	m_camera->uploadCamera(shaderSet);
	m_camera->updateCamera();
	for (auto& x : m_objects) {
		x.second->renderBoundingVolume(shaderSet);
	}
}

void Scene::renderLights(ShaderSet* shaderSet)
{
	shaderSet->UseProgram();
	m_camera->uploadCamera(shaderSet);
	m_camera->updateCamera();
	for (auto& l : m_lights)
		l.second->renderMesh(shaderSet);
}

void Scene::addMesh(Mesh* mesh)
{
	m_meshes.push_back(mesh);
}

void Scene::addObject(Object* object)
{
	m_objects[object->getName()] = object;
	//Children missing
	m_vertexCount += object->getMesh()->getPoints();
	m_triangleCount += object->getMesh()->getIndices()/3;
	m_objectNames.push_back(object->getName());
}

void Scene::addLight(std::string name, Lightsource* light)
{
	m_lights[name] = light;
	m_lightNames.push_back(name);
}

Object* Scene::findObject(std::string name)
{
	if (m_objects.find(name) == m_objects.end()) {
		for (auto& o : m_objects) {
			if(o.second->findObject(name)!= nullptr)
				return o.second->findObject(name);
		}
		return nullptr;
	}
	else 
		return m_objects[name];
}

Lightsource* Scene::findLight(std::string name)
{
	if (m_lights.find(name) == m_lights.end())
		return nullptr;
	else
		return m_lights[name];
}

void Scene::setCamera(Camera* camera)
{
	m_camera = camera;
}

unsigned int Scene::getVertexCount()
{
	return m_vertexCount;
}

unsigned int Scene::getTriangleCount()
{
	return m_triangleCount;
}


void Scene::prepareScene()
{
	prepareLightBuffer();
}

void Scene::prepareLightBuffer()
{
	glCreateBuffers(1, &m_bufferID);
	updateLightBuffer();
}



void Scene::updateLightBuffer()
{
	if (m_lights.size() > MAX_LIGHTS)
		std::cout << "To many lights!" << std::endl;
	else {
		int i = 0;
		for (auto& l : m_lights) {
			l.second->updateLightMatrix();
			m_lightStructs[i] = l.second->getLightStruct();
			i++;
		}
		glNamedBufferStorage(m_bufferID, MAX_LIGHTS * sizeof(LightStruct), NULL, GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
		void* posptr = glMapNamedBuffer(m_bufferID, GL_READ_WRITE);
		memcpy(posptr, m_lightStructs, sizeof(m_lightStructs));
		glUnmapNamedBuffer(m_bufferID);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_bufferID);
	}
}

