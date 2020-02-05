#include "RigidBodyPhysics.h"
#define EPSILON 0.003f
RigidBodyPhysics::RigidBodyPhysics()
{

}

void RigidBodyPhysics::addRigidBody(RigidBody* r)
{
	m_rigidBodys.push_back(r);
	//r->applyForce(m_gravity);
}

void RigidBodyPhysics::update(float dt)
{
	//Collision Check
	//std::vector<Collision*> collisions;
	for (int i = 0; i < m_rigidBodys.size(); i++) {
		m_rigidBodys[i]->clearForce();
		m_rigidBodys[i]->clearTorque();
		m_rigidBodys[i]->applyForce(m_gravity * m_rigidBodys[i]->getMass());
		for (int j = 0; j < m_rigidBodys.size(); j++) {
			if (i <= j) // Collision with itself
				continue;
			Collision* c = checkCollision(m_rigidBodys[i], m_rigidBodys[j]);
			if (c == nullptr) // no Collision
				continue;
			//collisions.push_back(c);
			calculateCollision(c);
		}
	}
	//Update Bodys
	/*for (Collision* c : collisions) {
		calculateCollision(c);
	}
	collisions.clear();*/
	for (int i = 0; i < m_rigidBodys.size();i++) {
		m_rigidBodys[i]->integrate(dt);
	}
}

std::vector<RigidBody*>* RigidBodyPhysics::getRigidBodyArray()
{
	return &m_rigidBodys;
}

Collision* RigidBodyPhysics::checkCollision(RigidBody* i, RigidBody* j)
{
	if (i->getShape()->getType() == SPHERE && j->getShape()->getType() == SPHERE) {
		float r1 = static_cast<Sphere*>(i->getShape())->getRadius();
		glm::vec3 pos1 = i->getShape()->getPosition();
		float r2 = static_cast<Sphere*>(j->getShape())->getRadius();
		glm::vec3 pos2 = j->getShape()->getPosition();
		glm::vec3 normal = glm::normalize(pos1 - pos2);
		float d = glm::dot(i->getLinearVelocity() - j->getLinearVelocity(), normal);
		if (glm::distance(pos1, pos2) <= r1 + r2 && d <= 0.0f) {
			Collision* c = new Collision();
			c->normal = normal;
			c->contact = pos1 + c->normal * r1;
			c->i = i;
			c->j = j;
			c->d = d;
			return c;
		}
		return nullptr;
	}
	else if (i->getShape()->getType() == SPHERE && j->getShape()->getType() == PLANE) {
		float r1 = static_cast<Sphere*>(i->getShape())->getRadius();
		glm::vec3 pos1 = i->getShape()->getPosition();
		glm::vec3 pos2 = j->getShape()->getPosition();
		glm::vec3 normal = glm::normalize(static_cast<Plane*>(j->getShape())->getNormal());
		glm::vec3 posOnPlane = pos1 - glm::dot(pos1 - pos2, normal) * normal;
		float distSpherePlane = glm::length(posOnPlane - pos1);
		float d = glm::dot(i->getLinearVelocity() - j->getLinearVelocity(), normal);
		if (distSpherePlane <= r1 && d <= 0.0f) {
			Collision* c = new Collision();
			c->contact = posOnPlane;
			c->normal = normal;
			c->i = i;
			c->j = j;
			c->d = d;
			return c;
		}
		return nullptr;
	}
	else if (i->getShape()->getType() == PLANE && j->getShape()->getType() == SPHERE) {
		float r1 = static_cast<Sphere*>(j->getShape())->getRadius();
		glm::vec3 pos1 = j->getShape()->getPosition();
		glm::vec3 pos2 = i->getShape()->getPosition();
		glm::vec3 normal = glm::normalize(static_cast<Plane*>(i->getShape())->getNormal());
		glm::vec3 posOnPlane = pos1 - glm::dot(pos1 - pos2, normal) * normal;
		float distSpherePlane = glm::length(posOnPlane - pos1);
		float d = glm::dot(j->getLinearVelocity() - i->getLinearVelocity(), normal);
		if (distSpherePlane <= r1 && d <= 0.0f) {
			Collision* c = new Collision();
			c->contact = posOnPlane;
			c->normal = normal;
			c->i = i;
			c->j = j;
			c->d = d;
			return c;
		}
		return nullptr;
	}
	else {
		return nullptr;
	}
}

void RigidBodyPhysics::calculateCollision(Collision* c)
{
	glm::vec3 rA = c->contact - c->i->getShape()->getPosition();
	glm::vec3 rB = c->contact - c->j->getShape()->getPosition();
	glm::vec3 ra_cross_n = glm::cross(rA, c->normal);
	glm::vec3 rb_cross_n = glm::cross(rB, c->normal);

	float s = -(1.0 + c->i->getBounciness()) * (glm::dot(c->normal, c->i->getLinearVelocity() - c->j->getLinearVelocity()) + (glm::dot(c->i->getLinearVelocity(), ra_cross_n) - glm::dot(c->j->getAngularVelocity(), rb_cross_n)));
	float t = c->i->getMassInverse() + c->j->getMassInverse() + glm::dot(ra_cross_n * c->i->getLocalInertiaTensorInverse(), ra_cross_n) + glm::dot(rb_cross_n * c->j->getLocalInertiaTensorInverse(), rb_cross_n);
	float p = s / t;

		
	//Friction
	//glm::vec3 vr = (c->j->getLinearVelocity() - c->i->getLinearVelocity());
	//glm::vec3 tangent = glm::normalize(vr - glm::dot(vr,c->normal) * c->normal);
	//float s = -(1.0 + c->i->getBounciness()) * (glm::dot(tangent, c->i->getLinearVelocity() - c->j->getLinearVelocity()));
	//float t = c->i->getMassInverse() + c->j->getMassInverse();
	//float p = s / t;

	//float jt = -glm::dot(vr, tangent);
	//jt /= c->i->getMassInverse() + c->j->getMassInverse();
	//float mu_static = glm::sqrt(c->i->getStaticFriction() * c->i->getStaticFriction() + c->j->getStaticFriction() * c->j->getStaticFriction());

	//glm::vec3 frictionImpuls;
	//if (glm::abs(jt) < p * mu_static)
	//	frictionImpuls = jt * tangent;
	//else {
	//	float mu_dynamic = glm::sqrt(c->i->getDynamicFriction() * c->i->getDynamicFriction() + c->j->getDynamicFriction() * c->j->getDynamicFriction());
	//	frictionImpuls = -p * tangent * mu_dynamic;
	//}
	//c->i->setLinearVelocity(c->i->getLinearVelocity() + (p * c->i->getMassInverse()) * frictionImpuls);
	//c->j->setLinearVelocity(c->j->getLinearVelocity() - (p * c->j->getMassInverse()) * frictionImpuls);

	//Rotation
	//glm::vec3 rA_cross_pn = glm::cross(c->normal * p, rA);
	//glm::vec3 rB_cross_pn = glm::cross(c->normal* p,rB);
	//c->i->setAngularVelocity(c->i->getAngularVelocity() + c->i->getLocalInertiaTensorInverse() * rA_cross_pn);
	//c->j->setAngularVelocity(c->j->getAngularVelocity() - c->j->getLocalInertiaTensorInverse() * rB_cross_pn);

	//Translation
	c->i->setLinearVelocity(c->i->getLinearVelocity() + (p * c->i->getMassInverse()) * c->normal);
	c->j->setLinearVelocity(c->j->getLinearVelocity() - (p * c->j->getMassInverse()) * c->normal);
}

