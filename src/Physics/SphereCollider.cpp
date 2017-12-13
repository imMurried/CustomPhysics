#include "SphereCollider.h"
#include "PhysicsObject.h"

using namespace Physics;

SphereCollider::SphereCollider() : m_radius(1.0f), Collider(Type::SPHERE)
{
}

SphereCollider::SphereCollider(float radius) : m_radius(radius), Collider(Type::SPHERE)
{
}


SphereCollider::~SphereCollider()
{
}

bool SphereCollider::Intersects(Collider * other, IntersectData * intersect) const
{
	switch (other->GetType())
	{
	case Type::SPHERE:
		return SphereToSphereIntersect(this, (SphereCollider*)other, intersect);
	case Type::AABB:
		return SphereToAABBIntersect(this, (AABBCollider*)other, intersect);
		break;
	}
	return false;
}

void Physics::SphereCollider::Transform(Object * obj)
{
	SetPosition(obj->GetPosition());
}
