#pragma once
#include "Intersect.h"

namespace Physics
{
	class Object;
	class SphereCollider;
	class AABBCollider;

	class Collider
	{
	public:

		enum class Type
		{
			NONE,
			SPHERE,
			AABB,
			OBB
		};

		Collider(Type type);
		virtual ~Collider();

		Type GetType() const { return m_type; };

		virtual void Transform(Object* obj) { /* Intentionally left blank */};
		virtual bool Intersects(Collider* other, IntersectData* intersect) const { return false; };

		// static functions: A single function no matter how many
		// colliders are created
		static Collider* GetNullInstance();

		static bool SphereToSphereIntersect(const SphereCollider *objA, const SphereCollider *objB, IntersectData* intersect);
		static bool AABBToAABBIntersect(const AABBCollider *objA, const AABBCollider *objB, IntersectData* intersect);
		static bool AABBToSphereIntersect(const AABBCollider *objA, const SphereCollider *objB, IntersectData* intersect);
		static bool SphereToAABBIntersect(const SphereCollider *objA, const AABBCollider * objB, IntersectData* intersect);

	protected:
		Type m_type;
	private:
	};
}

