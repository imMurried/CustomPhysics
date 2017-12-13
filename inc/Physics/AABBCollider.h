#pragma once

#include "Collider.h"
#include "Intersect.h"

#include <glm\vec3.hpp>

namespace Physics
{
	class Object;

	class AABBCollider : public Collider
	{
	public:
		AABBCollider();
		AABBCollider(glm::vec3 dim);
		virtual ~AABBCollider();

		// getters
		const glm::vec3 & GetPosition() const { return m_position; }
		const glm::vec3 & GetDimensions() const { return m_dimensions; }
		const glm::vec3 & GetExtents() const { return m_extents; };

		// setters
		void SetPosition(const glm::vec3 & position) { m_position = position; }
		void SetDimensions(glm::vec3 dim) { m_dimensions = dim; }

		bool Intersects(Collider* other, IntersectData* intersect) const;

		virtual void Transform(Object *obj);

	protected:
		glm::vec3 m_position;
		glm::vec3 m_dimensions;
		glm::vec3 m_extents;
	private:
	};
}
