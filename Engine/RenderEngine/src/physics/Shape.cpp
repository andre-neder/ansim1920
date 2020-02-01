#include "Shape.h"


Sphere::Sphere(glm::vec3 position, glm::mat3 rotation, float radius) : Shape(position,rotation)
{
	m_type = SPHERE;
	m_radius = radius;
}

float Sphere::getRadius()
{
	return m_radius;
}

Plane::Plane(glm::vec3 position, glm::mat3 rotation, glm::vec3 normal) : Shape(position, rotation)
{
	m_type = PLANE;
	m_normal = normal;
}

glm::vec3 Plane::getNormal()
{
	return m_normal;
}

void Shape::setPosition(glm::vec3 position)
{
	m_position = position;
}

void Shape::setRotation(glm::mat3 rotation)
{
	m_rotation = rotation;
}

int Shape::getType()
{
	return m_type;
}


glm::vec3 Shape::getPosition()
{
	return m_position;
}

glm::mat3 Shape::getRotation()
{
	return m_rotation;
}

Shape::Shape(glm::vec3 position, glm::mat3 rotation)
{
	m_position = position;
	m_rotation = rotation;
}
