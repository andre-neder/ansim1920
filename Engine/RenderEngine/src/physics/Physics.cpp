#include "Physics.h"

Physics::Physics()
{
	m_gravity = glm::vec3(0.0, -9.81, 0.0);
}

glm::vec3 Physics::getGravity()
{
	return m_gravity;
}

