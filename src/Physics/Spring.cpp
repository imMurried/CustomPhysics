#include "Spring.h"
#include <glm/vec3.hpp>
#include "PhysicsObject.h"
#include <glm/geometric.hpp>

using namespace Physics;

Physics::Spring::Spring(Object * objA, Object * objB) :
	Constraint(std::vector<Object*> { objA, objB }, Constraint::Type::SPRING),
	m_length(5.0f),
	m_stiffness(100.0f),
	m_dampening(1.0f)
{

}

Physics::Spring::Spring(Object * objA, Object * objB, float length, float stiffness, float dampening) :
	Constraint(std::vector<Object*> { objA, objB }, Constraint::Type::SPRING),
	m_length(length),
	m_stiffness(stiffness),
	m_dampening(dampening)
{
}

Spring::~Spring()
{
}

void Physics::Spring::Update()
{
	// all springs have exatcly 2 objects in their vector
	Object* objA = m_objects[0];
	Object* objB = m_objects[1];

	// measure distance between two objects
	glm::vec3 springVec = objB->GetPosition() - objA->GetPosition();
	float distance = glm::length(springVec);

	// create a force along the vector, based on how far away we are from our length
	glm::vec3 force;

	if (distance != m_length)
	{
		force = glm::normalize(springVec) * (distance - m_length) * m_stiffness;
		// edit that force for friction (the faster things are moving, the more we slow them)
		
		force += glm::dot(objB->GetVelocity() - objA->GetVelocity(), springVec) * glm::normalize(springVec) * m_dampening;
	}


	// apply force to both objects in the required direction
	objA->ApplyForce(force);
	objB->ApplyForce(-force);

}
