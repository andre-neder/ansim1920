#pragma once
#include "RigidBody.h"
class StaticBody : public RigidBody {
public:
	StaticBody(Object* object, int shape);
	void integrate(float dt) override;
	void applyForce(glm::vec3 force) override;
	void applyImpuls(glm::vec3 p) override;
	void applyTorque(glm::vec3 torque) override;
	void setInitialVelocity(glm::vec3 v);
	void setInitialAngularVelocity(glm::vec3 w) override;
	void setLinearVelocity(glm::vec3 v);
	void setLinearMomentum(glm::vec3 p);
	void setAngularVelocity(glm::vec3 w);
};