#pragma once
#include "../Object.h"
#include "Shape.h"

class RigidBody {
public:
	//Constructor
	RigidBody(Object* object, int shape);
	//update
	virtual void integrate(float dt) = 0;
	virtual void applyForce(glm::vec3 force) = 0;
	virtual void setInitialVelocity(glm::vec3 v) = 0;
	void setLinearVelocity(glm::vec3 v);
	void setLinearMomentum(glm::vec3 p);
	void setAngularVelocity(glm::vec3 w);
	//Getters
	Object* getObject();
	Shape* getShape();
	
	glm::vec3 getLinearVelocity();
	glm::vec3 getAngularVelocity();
	glm::vec3 getLinearMomentum();
	glm::vec3 getForce();

	glm::mat3 getLocalInertiaTensorInverse();
	
	float getBounciness();
	float getFrictionCoefficient();
	float getMass();
	float getMassInverse();
protected:
	Object* m_object;
	Shape* m_shape;
	
	float m_mass;
	float m_massInverse;
	float m_bounciness;
	float m_frictionCoefficient;

	glm::vec3 m_linearVelocity;
	glm::vec3 m_linearAcceleration;
	glm::vec3 m_linearMomentum;
	glm::vec3 m_force;
	
	glm::vec3 m_angularVelocity;
	glm::vec3 m_angularAcceleration;
	glm::vec3 m_angularMomentum;
	glm::vec3 m_torque;

	glm::mat3 m_localInertiaTensor;
	glm::mat3 m_localInertiaTensorInverse;
};
