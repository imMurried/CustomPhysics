#pragma once

#include "Collider.h"
#include "Intersect.h"

#include <glm\vec3.hpp>

namespace Physics
{
	class Object;

	class SphereCollider : public Collider
	{
	public:
		SphereCollider();
		SphereCollider(float radius);
		virtual ~SphereCollider();

		// getters
		const glm::vec3 & GetPosition() const { return m_position; }
		float GetRadius() const { return m_radius; }

		// setters
		void SetPosition(const glm::vec3 & position) { m_position = position; }
		void SetRadius(float radius) { m_radius = radius; }

		bool Intersects(Collider* other, IntersectData* intersect) const;

		virtual void Transform(Object *obj);

	protected:
		glm::vec3 m_position;
		float m_radius;

	private:
	};
}


