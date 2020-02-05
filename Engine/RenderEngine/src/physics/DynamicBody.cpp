#include "DynamicBody.h"
#include "glm/gtx/quaternion.hpp"
#define EPSILON 0.003f

DynamicBody::DynamicBody(Object* object, int shape) : RigidBody(object, shape)
{
	
	m_type = DYNAMIC;
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
	m_type = DYNAMIC;
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
	if (glm::abs(m_linearVelocity).x < EPSILON)
		m_linearVelocity.x = 0.0;
	//if (glm::abs(m_linearVelocity).y < EPSILON)
		m_linearVelocity.y = 0.0;
	if (glm::abs(m_linearVelocity).z < EPSILON)
		m_linearVelocity.z = 0.0;
	//if (glm::abs(m_angularVelocity).x < EPSILON)
	//	m_angularVelocity.x = 0.0;
	//if (glm::abs(m_angularVelocity).y < EPSILON)
	//	m_angularVelocity.y = 0.0;
	//if (glm::abs(m_angularVelocity).z < EPSILON)
	//	m_angularVelocity.z = 0.0;
	glm::vec3 oldPos = m_shape->getPosition();
	glm::vec3 newPos = oldPos + m_linearVelocity * dt;
	m_object->setPosition(newPos);
	m_shape->setPosition(newPos);
	m_linearAcceleration = m_force * m_massInverse;
	m_linearVelocity += m_linearAcceleration * dt;
	//Rotation
	glm::quat q = glm::toQuat(m_shape->getRotation());
	glm::quat qNext = q + dt * 0.5f * glm::quat(0,m_angularVelocity) * q;
	qNext = glm::normalize(qNext);
	glm::mat3 R = glm::toMat3(qNext);
	m_shape->setRotation(R);
	m_object->rotate(R);

	m_angularMomentum += m_torque * dt;

	m_angularVelocity += R * m_localInertiaTensorInverse * glm::transpose(R) * m_angularMomentum;
	//Angular Dampening
	//glm::vec3 dampSpin = glm::vec3(0.0, -15.0, 0.0);
	//if (m_angularVelocity.y > EPSILON)
	//	m_angularVelocity += R * m_localInertiaTensorInverse * glm::transpose(R) * dampSpin *dt *dt ;

}
void DynamicBody::applyForce(glm::vec3 force)
{
	m_force += force;
}

void DynamicBody::applyTorque(glm::vec3 torque)
{
	m_torque += torque;
}

void DynamicBody::setInitialVelocity(glm::vec3 v)
{
	m_linearVelocity = v;
}

void DynamicBody::setInitialAngularVelocity(glm::vec3 w)
{
	m_angularVelocity = w;
}

void DynamicBody::setMass(float mass)
{
	if (mass != 0.0) {
		m_mass = mass;
		m_massInverse = 1 / m_mass;
	}
}
