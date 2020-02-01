#include "Object.h"

Object::Object()
{
	m_modelMatrix = glm::mat4(1.0);
	m_centerOfMass = glm::vec3(0.0);
	m_mesh = NULL;
	m_material = NULL;
	m_boundingVolume = NULL;
	m_parent = nullptr;
	m_rotationMatrix = glm::mat4(1.0);
}

Object::~Object()
{
	m_material->~Material();
	m_mesh->~Mesh();
	m_boundingVolume->~Mesh();
}

Object::Object(std::string name, Mesh* mesh, Material* material)
{
	m_mesh = mesh;
	m_name = name;
	m_material = material;
	m_centerOfMass = glm::vec3(0.0);
	m_modelMatrix = glm::mat4(1.0);
	m_parent = NULL;
	m_rotationMatrix = glm::rotate(glm::mat4(1.0), 0.0f, glm::vec3(1.0));
	createBoundingVolume();
}

void Object::setTransforms(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	m_position = position;
	setOrientation(rotation);
	m_scale = scale;
	updateModelMatrix();
}

void Object::setPosition(glm::vec3 position)
{
	m_position = position;
	updateModelMatrix();
}

glm::vec3 Object::getPosition()
{
	return m_position;
}

void Object::setOrientation(glm::vec3 orientation)
{
	m_orientation = orientation;
	m_orientationMatrix = glm::rotate(glm::mat4(1.0), glm::radians(m_orientation.x), glm::vec3(1.0, 0.0, 0.0));
	m_orientationMatrix *= glm::rotate(glm::mat4(1.0), glm::radians(m_orientation.y), glm::vec3(0.0, 1.0, 0.0));
	m_orientationMatrix *= glm::rotate(glm::mat4(1.0), glm::radians(m_orientation.z), glm::vec3(0.0, 0.0, 1.0));
	updateModelMatrix();
}

void Object::rotate(glm::vec3 axis, float degree)
{
	m_rotationMatrix *= glm::rotate(glm::mat4(1.0), glm::radians(degree), axis);
}

void Object::rotate(glm::mat3 rotation)
{
	m_rotationMatrix = glm::mat4(rotation);
}

void Object::rotate()
{
	m_rotationMatrix *= m_rotationMatrix;
}

void Object::setScale(glm::vec3 scale)
{
	m_scale = scale;
	updateModelMatrix();
}



glm::vec3 Object::getScale()
{
	return m_scale;
}

void Object::setMaterial(Material* material)
{
	m_material = material;
}

void Object::render(ShaderSet* shaderSet){
	for (auto& c : m_children) {
		c.second->render(shaderSet);
	}
	glm::mat4 modelMatrix = m_modelMatrix;
	glm::mat4 normalMatrix = m_normalMatrix;
	if (m_parent != NULL) {
		modelMatrix = m_parent->getModelMatrix() * m_modelMatrix;
		normalMatrix = glm::transpose(glm::inverse(modelMatrix));
	}
	glUniformMatrix4fv(glGetUniformLocation(shaderSet->getProgramID(), "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shaderSet->getProgramID(), "normalMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	m_material->uploadMaterial(shaderSet);
	m_mesh->render();
}

void Object::renderMesh(ShaderSet* shaderSet){
	for (auto& c : m_children) {
		c.second->renderMesh(shaderSet);
	}
	glm::mat4 modelMatrix = m_modelMatrix;
	if (m_parent != NULL) {
		modelMatrix = m_parent->getModelMatrix() * m_modelMatrix;
	}
	glUniformMatrix4fv(glGetUniformLocation(shaderSet->getProgramID(), "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	m_mesh->render();
}

void Object::renderBoundingVolume(ShaderSet* shaderSet){
	for (auto& c : m_children) {
		c.second->renderBoundingVolume(shaderSet);
	}
	glm::mat4 modelMatrix = m_modelMatrix;
	if (m_parent != NULL) {
		modelMatrix = m_parent->getModelMatrix() * m_modelMatrix;
	}
	glUniformMatrix4fv(glGetUniformLocation(shaderSet->getProgramID(), "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	m_boundingVolume->render(GL_LINES);
}

void Object::setParent(Object* parent)
{
	m_parent = parent;
}

void Object::addChild(Object* child)
{
	child->setParent(this);
	//m_children.push_back(child);
	m_children[child->getName()] = child;
}

Object* Object::findObject(std::string name)
{
	if (m_children.find(name) == m_children.end()) {
		for (auto& o : m_children) {
			if (o.second->findObject(name) != nullptr)
				return o.second->findObject(name);
		}
		return nullptr;
	}
	else
		return m_children[name];
}


Mesh* Object::getMesh()
{
	return m_mesh;
}

std::string Object::getName()
{
	return m_name;
}

glm::mat4 Object::getModelMatrix()
{
	return m_modelMatrix;
}

glm::mat4 Object::getRotationMatrix()
{
	return m_rotationMatrix;
}

glm::mat4 Object::getNormalMatrix()
{
	return m_normalMatrix;
}

void Object::createBoundingVolume()
{
	glm::vec3 min = m_mesh->getMin();
	glm::vec3 max = m_mesh->getMax();
	m_centerOfMass = max - min;
	std::vector<glm::vec4> vertices;
	vertices.push_back(glm::vec4(min.x, min.y, min.z,1.0));  // vorne links unten
	vertices.push_back(glm::vec4(max.x, min.y, min.z, 1.0)); // vorne rechts unten
	vertices.push_back(glm::vec4(min.x, max.y, min.z, 1.0)); // vorne links oben
	vertices.push_back(glm::vec4(max.x, max.y, min.z, 1.0)); // vorne rechts oben
	vertices.push_back(glm::vec4(min.x, min.y, max.z, 1.0)); // hinten links unten
	vertices.push_back(glm::vec4(max.x, min.y, max.z, 1.0)); // hinten rechts unten
	vertices.push_back(glm::vec4(max.x, max.y, max.z, 1.0)); // hinten links oben
	vertices.push_back(glm::vec4(min.x, max.y, max.z, 1.0)); // hinten rechts oben
	std::vector<unsigned int> indices = { 0,1,1,3,3,2,2,0,0,4,1,5,3,6,2,7,7,6,6,5,5,4,4,7 };
	m_boundingVolume = new Mesh(vertices,indices);
}

void Object::updateModelMatrix()
{
	//inital orientation
	glm::mat4 rotationMatrix = m_orientationMatrix;
	//Rotation
	rotationMatrix *= m_rotationMatrix;
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), m_scale);
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0), m_position);
	m_modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	m_normalMatrix = glm::transpose(glm::inverse(m_modelMatrix));
}
