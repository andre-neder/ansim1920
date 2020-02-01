#pragma once
#include "RigidBody.h"

class DynamicBody : public RigidBody {
public:
	DynamicBody(Object* object, int shape);
	DynamicBody(Object* object, int shape, float mass);
	void integrate(float dt) override;
	void applyForce(glm::vec3 force) override;
	void setInitialVelocity(glm::vec3 v) override;
	void setMass(float mass);
};