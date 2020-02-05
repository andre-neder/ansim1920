#include "StaticBody.h"

StaticBody::StaticBody(Object* object, int shape) : RigidBody(object,shape)
{
	m_type = STATIC;
	m_mass = 1.0;
	m_massInverse = 0.0;
	m_localInertiaTensor = glm::mat3(1.0);
	m_localInertiaTensorInverse = glm::mat3(0.0);
}

void StaticBody::integrate(float dt)
{
	//Do nothing
}

void StaticBody::applyForce(glm::vec3 force)
{
	//Do nothing
}

void StaticBody::applyImpuls(glm::vec3 p)
{
}

void StaticBody::applyTorque(glm::vec3 torque)
{
}

void StaticBody::setInitialVelocity(glm::vec3 v)
{
	//Do Nothing
}

void StaticBody::setInitialAngularVelocity(glm::vec3 w)
{
}

void StaticBody::setLinearVelocity(glm::vec3 v)
{
}

void StaticBody::setLinearMomentum(glm::vec3 p)
{
}

void StaticBody::setAngularVelocity(glm::vec3 w)
{
}
