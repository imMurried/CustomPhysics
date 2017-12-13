#include "AABBCollider.h"
#include "PhysicsObject.h"

using namespace Physics;

AABBCollider::AABBCollider() : m_dimensions(1, 1, 1), m_extents(m_dimensions * 0.5f), Collider(Type::AABB)
{
}

Physics::AABBCollider::AABBCollider(glm::vec3 dim) : m_dimensions(dim), m_extents(dim * 0.5f), Collider(Type::AABB)
{
}


AABBCollider::~AABBCollider()
{
}

bool Physics::AABBCollider::Intersects(Collider * other, IntersectData * intersect) const
{
	switch (other->GetType())
	{
	case Type::AABB:
		return AABBToAABBIntersect(this, (AABBCollider*)other, intersect);
	case Type::SPHERE:
		return AABBToSphereIntersect(this, (SphereCollider*)other, intersect);
	}
	return false;
}

void Physics::AABBCollider::Transform(Object * obj)
{
	SetPosition(obj->GetPosition());
}
