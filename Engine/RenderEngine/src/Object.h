#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/ext.hpp"
#include "Mesh.h"
#include "Material.h"
#include "ShaderSet.h"
#include <map>

class Object {
public:
	Object();
	~Object();
	Object(std::string name, Mesh* mesh, Material* material);

	void setTransforms(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	void setPosition(glm::vec3 position);
	glm::vec3 getPosition();
	void setOrientation(glm::vec3 orientation);
	void rotate(glm::vec3 axis, float degree);
	void rotate(glm::mat3 rotation);
	void rotate();
	void setScale(glm::vec3 scale);
	glm::vec3 getScale();

	void setMaterial(Material* material);

	void render(ShaderSet* shaderSet);
	void renderMesh(ShaderSet* shaderSet);
	void renderBoundingVolume(ShaderSet* shaderSet);

	void setParent(Object* parent);
	void addChild(Object* child);
	Object* findObject(std::string name);

	Mesh* getMesh();
	std::string getName();
	glm::mat4 getModelMatrix();
	glm::mat4 getRotationMatrix();
	glm::mat4 getNormalMatrix();

private:
	//Mesh
	Mesh* m_mesh;
	Material* m_material;
	std::string m_name;

	//Bounding Volume
	void createBoundingVolume();
	Mesh* m_boundingVolume;
	glm::vec3 m_centerOfMass;
	
	//Inheritance
	std::map<std::string, Object*> m_children;
	Object* m_parent;

	//Transforms
	glm::vec3 m_position;
	glm::vec3 m_orientation;
	glm::vec3 m_scale;

	//Matrices
	void updateModelMatrix();
	glm::mat4 m_modelMatrix;
	glm::mat4 m_orientationMatrix;
	glm::mat4 m_rotationMatrix;
	glm::mat4 m_normalMatrix;
};