#include "Collider.h"
#include "SphereCollider.h"
#include "AABBCollider.h"
#include "PhysicsScene.h"

#include <glm\vec3.hpp>
#include<glm\geometric.hpp>

using namespace Physics;

Collider::Collider(Type type) : m_type(type)
{
}


Collider::~Collider()
{
}

Collider * Physics::Collider::GetNullInstance()
{
	// static object will be created once, then every time
	// you try to create it again, it will simply reference
	// the one that was created.
	static Collider collider(Type::NONE);
	return &collider;
}

bool Physics::Collider::SphereToSphereIntersect(const SphereCollider * objA, const SphereCollider * objB, IntersectData* intersect)
{
	glm::vec3 collisionvector = objB->GetPosition() - objA->GetPosition();
	// calculate distance and total of both radii so that we can determine if intersecting
	float distance = glm::length(collisionvector);
	float colDistance = objA->GetRadius() + objB->GetRadius();

	// create the collision vector which points from A to B and is the length of the overlap
	collisionvector = glm::normalize(collisionvector) * (colDistance - distance);
	intersect->collisionVector = collisionvector;

	return distance < colDistance;

}

bool Physics::Collider::AABBToAABBIntersect(const AABBCollider * objA, const AABBCollider * objB, IntersectData* intersect)
{
	glm::vec3 collisionVector = objB->GetPosition() - objA->GetPosition();

	glm::vec3 colVector(abs(objB->GetExtents().x) + abs(objA->GetExtents().x),
						abs(objB->GetExtents().y) + abs(objA->GetExtents().y),
						abs(objB->GetExtents().z) + abs(objA->GetExtents().z));
	
	glm::vec3 infoVector = glm::normalize(collisionVector);

	infoVector.x *= colVector.x - abs(collisionVector.x);
	infoVector.y *= colVector.y - abs(collisionVector.y);
	infoVector.z *= colVector.z - abs(collisionVector.z);

	intersect->collisionVector = infoVector;

	if (abs(collisionVector.x) < colVector.x
		&& abs(collisionVector.y) < colVector.y
		&& abs(collisionVector.z) < colVector.z)
	{
		return true;
	}

	return false;
}

bool Physics::Collider::AABBToSphereIntersect(const AABBCollider * objA, const SphereCollider * objB, IntersectData * intersect)
{
	glm::vec3 collisionVector = objB->GetPosition() - objA->GetPosition();

	glm::vec3 colVector(objB->GetRadius() + abs(objA->GetExtents().x),
		objB->GetRadius() + abs(objA->GetExtents().y),
		objB->GetRadius() + abs(objA->GetExtents().z));

	glm::vec3 infoVector = glm::normalize(collisionVector);

	infoVector.x *= colVector.x - abs(collisionVector.x);
	infoVector.y *= colVector.y - abs(collisionVector.y);
	infoVector.z *= colVector.z - abs(collisionVector.z);

	intersect->collisionVector = infoVector;

	if (abs(collisionVector.x) < colVector.x
		&& abs(collisionVector.y) < colVector.y
		&& abs(collisionVector.z) < colVector.z)
	{
		return true;
	}

	return false;
}

bool Physics::Collider::SphereToAABBIntersect(const SphereCollider * objA, const AABBCollider * objB, IntersectData * intersect)
{
	glm::vec3 collisionVector = objB->GetPosition() - objA->GetPosition();

	glm::vec3 colVector(abs(objB->GetExtents().x) + objA->GetRadius(),
		abs(objB->GetExtents().y) + objA->GetRadius(),
		abs(objB->GetExtents().z) + objA->GetRadius());

	glm::vec3 infoVector = glm::normalize(collisionVector);

	infoVector.x *= colVector.x - abs(collisionVector.x);
	infoVector.y *= colVector.y - abs(collisionVector.y);
	infoVector.z *= colVector.z - abs(collisionVector.z);

	intersect->collisionVector = infoVector;

	if (abs(collisionVector.x) < colVector.x
		&& abs(collisionVector.y) < colVector.y
		&& abs(collisionVector.z) < colVector.z)
	{
		return true;
	}

	return false;
}


