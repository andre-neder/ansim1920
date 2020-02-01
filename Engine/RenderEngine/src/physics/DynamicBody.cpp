#include "DynamicBody.h"
#include "glm/gtx/quaternion.hpp"

DynamicBody::DynamicBody(Object* object, int shape) : RigidBody(object, shape)
{
	if (shape == SPHERE) {
		float radius = static_cast<Sphere*>(m_shape)->getRadius();
		m_mass = 1.0;
		float J = 2.0f / 5.0f * m_mass * radius * radius;
		m_localInertiaTensor = glm::mat3(J);
	}
	else {
		m_mass = 1.0f;
		m_localInertiaTensor = glm::mat3(1.0);
	}
	m_massInverse = 1.0 / m_mass;
	m_localInertiaTensorInverse = glm::inverse(m_localInertiaTensor);
	m_angularVelocity = m_shape->getRotation() * m_localInertiaTensorInverse * glm::transpose(m_shape->getRotation()) * m_angularMomentum;
}

DynamicBody::DynamicBody(Object* object, int shape, float mass) : RigidBody(object, shape)
{
	if (shape == SPHERE) {
		float radius = static_cast<Sphere*>(m_shape)->getRadius();
		m_mass = mass;
		float J = 2.0f / 5.0f * m_mass * radius * radius;
		m_localInertiaTensor = glm::mat3(J);
	}
	else {
		m_mass = mass;
		m_localInertiaTensor = glm::mat3(1.0);
	}
	m_massInverse = 1.0 / m_mass;
	m_localInertiaTensorInverse = glm::inverse(m_localInertiaTensor);
	m_angularVelocity = m_shape->getRotation() * m_localInertiaTensorInverse * glm::transpose(m_shape->getRotation()) * m_angularMomentum;

}


void DynamicBody::integrate(float dt)
{
	//Translation
	glm::vec3 oldPos = m_shape->getPosition();
	glm::vec3 newPos = oldPos + m_linearVelocity * dt;
	m_object->setPosition(newPos);
	m_shape->setPosition(newPos);
	m_linearAcceleration = m_force * m_massInverse;
	m_linearVelocity += m_linearAcceleration * dt;
	//Rotation
	m_angularMomentum += m_torque * dt;
	glm::quat q = glm::toQuat(m_shape->getRotation());
	glm::quat qNext = q + dt * 0.5f * glm::quat(0,m_angularVelocity) * q;
	qNext = glm::normalize(qNext);
	glm::mat3 R = glm::toMat3(qNext);
	m_shape->setRotation(R);
	m_object->rotate(R);
	m_angularVelocity += R * m_localInertiaTensorInverse * glm::transpose(R) * m_angularMomentum;

}

void DynamicBody::applyForce(glm::vec3 force)
{
	m_force += force;
}

void DynamicBody::setInitialVelocity(glm::vec3 v)
{
	m_linearVelocity = v;
}

void DynamicBody::setMass(float mass)
{
	if (mass != 0.0) {
		m_mass = mass;
		m_massInverse = 1 / m_mass;
	}
}
