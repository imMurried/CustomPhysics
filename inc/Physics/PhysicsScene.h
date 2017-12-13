#pragma once

#include "Intersect.h"

#include <vector>

#include <glm\vec3.hpp>

namespace Physics
{
	class Object;
	class Constraint;

	class Scene
	{
	public:

		struct CollisionInfo
		{
			Object* objA;
			Object* objB;

			IntersectData intersect;
		};

		Scene();
		virtual ~Scene();

		void Update(float deltaTime);

		// Apply force to all objects
		void ApplyGlobalForce(const glm::vec3 & force);

		// attach a Physics Object to the scene
		void AttachObject(Object* obj);

		// remove a Physics Object from the scene
		void RemoveObject(Object* obj);

		const std::vector<Object*> & GetObjects() const;

		bool isObjectColliding(Object* obj);

		void AttachConstraint(Constraint* con);
		void RemoveConstraint(Constraint* con);
		const std::vector<Constraint *> & GetConstraints();

	protected:
		std::vector<Object *> m_objects;
		std::vector<Constraint *> m_constraints;
		glm::vec3 m_globalForce;
		glm::vec3 m_gravity = {0, -9.81, 0};

		void DetectCollisions();
		void CustomDetectCollisions();
		void ResolveCollisons();
		std::vector<CollisionInfo> m_collisions;
		//std::map<PhysicsObject *, bool> m_isCollidingLookup; // speeds up lookup for whether collisions have happened
															 // but it's totally not what we would do

	private:
	};
}

