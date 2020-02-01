#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/ext.hpp"
#include "Mesh.h"
#include "Material.h"
#include "Object.h"
#include "ShaderSet.h"
#include "Lightsource.h"
#include "Camera.h"
#include <vector>
#include "Instance.h"
#include <map>

#define MAX_LIGHTS 5

class Scene {
public:
	Scene();
	~Scene();
	void render(ShaderSet* shaderSet);
	void renderShadows(ShaderSet* shaderSet);
	void renderBoundingVolumes(ShaderSet* shaderSet);
	void renderLights(ShaderSet* shaderSet);

	void prepareScene();
	void updateLightBuffer();

	void addMesh(Mesh* mesh);
	void addObject(Object* object);
	void addLight(std::string name, Lightsource* light);
	void setCamera(Camera* camera);

	Object* findObject(std::string name);
	Lightsource* findLight(std::string name);
	std::vector<std::string>m_lightNames;
	std::vector<std::string>m_objectNames;

	unsigned int getVertexCount();
	unsigned int getTriangleCount();
private:
	void prepareLightBuffer();

	Camera* m_camera;
	Instance* m_instance;

	std::vector<Mesh*> m_meshes;
	std::vector<Material*> m_material;
	std::map<std::string, Object*> m_objects;

	GLuint m_bufferID;
	std::map<std::string, Lightsource*> m_lights;
	LightStruct m_lightStructs[MAX_LIGHTS];

	unsigned int m_vertexCount;
	unsigned int m_triangleCount;

};