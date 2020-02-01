#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/ext.hpp"
#include <iostream>
#include <string>
#include <vector>

#define SPHERE 1
#define PLANE 0

class Shape {
public:
	void setPosition(glm::vec3 position);
	void setRotation(glm::mat3 rotation);

	int getType();
	glm::vec3 getPosition();
	glm::mat3 getRotation();
protected:
	Shape(glm::vec3 position, glm::mat3 rotation);
	int m_type;
	glm::vec3 m_position;
	glm::mat3 m_rotation;
};

class Sphere : public Shape{
public:
	Sphere(glm::vec3 position,glm::mat3 rotation, float radius);
	float getRadius();
protected:
	float m_radius;
};

class Plane : public Shape {
public:
	Plane(glm::vec3 position, glm::mat3 rotation, glm::vec3 normal);
	glm::vec3 getNormal();
protected:
	glm::vec3 m_normal;
};
