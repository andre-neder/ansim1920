#pragma once
#include "Physics.h"
#include "RigidBody.h"
#include "StaticBody.h"
#include "DynamicBody.h"

struct Collision {
	RigidBody* i;
	RigidBody* j;
	glm::vec3 normal;
	glm::vec3 contact;
};

class RigidBodyPhysics : public Physics{
public:
	RigidBodyPhysics();
	void addRigidBody(RigidBody* r);
	void update(float dt);
	std::vector<RigidBody*>* getRigidBodyArray();
private:
	Collision* checkCollision(RigidBody* i, RigidBody* j);
	void calculateCollision(Collision* c);
	std::vector<RigidBody*> m_rigidBodys;
};