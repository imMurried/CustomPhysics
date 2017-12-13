#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Collider.h"
#include "Constraint.h"

#include <algorithm>

#include <glm/geometric.hpp>

#include <iostream>

using namespace Physics;

Scene::Scene()
{
}


Scene::~Scene()
{
	for (auto iter = m_objects.begin(); iter != m_objects.end(); iter++)
	{
		delete *iter;
	}
	m_objects.clear();

	for (auto iter = m_constraints.begin(); iter != m_constraints.end(); iter++)
	{
		delete *iter;
	}
	m_constraints.clear();
}

void Physics::Scene::Update(float deltaTime)
{
	// Update all constraints
	for (auto iter = m_constraints.begin(); iter!= m_constraints.end(); iter++)
	{
		(*iter)->Update();
	}

	for (auto iter = m_objects.begin(); iter != m_objects.end(); iter++)
	{
		Object *obj = *iter;
		obj->ApplyForce(m_globalForce);
		obj->SetAcceleration(m_gravity + obj->GetAcceleration());
		obj->Update(deltaTime);

		// Super dodgy ground collision bouncing
		// This will be replaced by proper collision detection and reaction later
		//-------------------------------------------------------------
		const glm::vec3 & pos = obj->GetPosition();
		const glm::vec3 & vel = obj->GetVelocity();

		if (pos.y < 0.0f)
		{
			obj->SetPosition(glm::vec3(pos.x, 0.0f, pos.z));
			obj->SetVelocity(glm::vec3(vel.x, -vel.y * 0.0f, vel.z));
		}

		//-------------------------------------------------------------

	}
	//m_globalForce = glm::vec3(); // option to turn off gravity each frame
									// which would mean you'd have to apply it
									// every frame
	//DetectCollisions();
	CustomDetectCollisions();
	ResolveCollisons();
}

void Physics::Scene::ApplyGlobalForce(const glm::vec3 & force)
{
	m_globalForce = force;
}

void Physics::Scene::AttachObject(Object * obj)
{
	auto iter = std::find(m_objects.begin(), m_objects.end(), obj);
	if (iter == m_objects.end()) // object pointer is not already in our vector
	{
		m_objects.push_back(obj);
	}
}

void Physics::Scene::RemoveObject(Object * obj)
{
	auto iter = std::find(m_objects.begin(), m_objects.end(), obj);
	if (iter != m_objects.end()) // object pointer is in our vector
	{
		delete *iter; // maybe this is needed, but maybe we just "give" the object back to another class
		m_objects.erase(iter);
	}
}

const std::vector<Object*>& Physics::Scene::GetObjects() const
{
	return m_objects;
}

bool Physics::Scene::isObjectColliding(Object * obj)
{
	// MAP

	// super inefficient searching . . . going to need to change this
	for (auto iter = m_collisions.begin(); iter != m_collisions.end(); iter++)
	{
		if ((*iter).objA == obj || (*iter).objB == obj)
			return true;
	}
	return false;
}

void Physics::Scene::AttachConstraint(Constraint * con)
{
	auto iter = std::find(m_constraints.begin(), m_constraints.end(), con);
	if (iter == m_constraints.end())
	{
		m_constraints.push_back(con);
	}
}

void Physics::Scene::RemoveConstraint(Constraint * con)
{
	auto iter = std::find(m_constraints.begin(), m_constraints.end(), con);
	if (iter != m_constraints.end())
	{
		delete *iter;
		m_constraints.erase(iter);
	}
}

const std::vector<Constraint*>& Physics::Scene::GetConstraints()
{
	return m_constraints;
}

void Physics::Scene::DetectCollisions()
{
	m_collisions.clear(); // remove old collisions from previous frame

	// Super inefficient brute force collision detection
	for (auto iterA = m_objects.begin(); iterA != m_objects.end(); iterA++)
	{
		Object* objA = *iterA;

		// iterB = iterA + 1 stops double checking
		// also stops it from checking itself
		for (auto iterB = iterA + 1; iterB != m_objects.end(); iterB++)
		{
			Object* objB = *iterB;

			CollisionInfo info;
			// do the colliders of the two objects overlap?
			if (objA->GetCollider()->Intersects(objB->GetCollider(), &info.intersect))
			{
				info.objA = objA;
				info.objB = objB;
				m_collisions.push_back(info);
			}
		}
	}
}

void Physics::Scene::CustomDetectCollisions()
{
	m_collisions.clear();

	const int maxX = 10;
	const int maxY = 3;
	const int maxZ = 10;

	std::vector<Object*>* list[maxX][maxY][maxZ] = {};

	//sort objects into buckets
	for (auto iterA = m_objects.begin(); iterA != m_objects.end(); iterA++)
	{
		Object* objA = *iterA;

		int x = ((int)objA->GetPosition().x / 5) + (int)(maxX / 2);
		int y = ((int)objA->GetPosition().y / 5) + (int)(maxY / 2);
		int z = ((int)objA->GetPosition().z / 5) + (int)(maxZ / 2);

		if (x >= 0 && x < maxX &&
			y >= 0 && y < maxY &&
			z >= 0 && z < maxZ)
		{
			if (list[x][y][z] == NULL)
			{
				list[x][y][z] = new std::vector<Object*>;
			}

			list[x][y][z]->push_back(objA);
		}
	}

	for (int x = 0; x < maxX; x++)
	{
		for (int y = 0; y < maxY; y++)
		{
			for (int z = 0; z < maxZ; z++)
			{
				if (list[x][y][z] != NULL)
				{
					for (auto iterA = list[x][y][z]->begin(); iterA != list[x][y][z]->end(); iterA++)
					{
						Object* objA = *iterA;

						for (auto iterB = iterA + 1; iterB != list[x][y][z]->end(); iterB++)
						{
							Object* objB = *iterB;

							CollisionInfo info;
							// do the colliders of the two objects overlap?
							if (objA->GetCollider()->Intersects(objB->GetCollider(), &info.intersect))
							{
								info.objA = objA;
								info.objB = objB;
								m_collisions.push_back(info);
							}
						}
					}
				}
				
			}
		}
	}

}

void Physics::Scene::ResolveCollisons()
{
	// Loop through all collision pairs
	for (auto iter = m_collisions.begin(); iter != m_collisions.end(); iter++)
	{
		// Get data from the collision
		// collision normal (direction of collision and overlap)
		glm::vec3 colNormal = iter->intersect.collisionVector;

		// mass of both objects
		float massA = iter->objA->GetMass();
		float massB = iter->objB->GetMass();

		// velocity of both objects
		glm::vec3 velA = iter->objA->GetVelocity();
		glm::vec3 velB = iter->objB->GetVelocity();

		// momentum
		glm::vec3 pA = massA * velA;
		glm::vec3 pB = massB * velB;

		// and relative velocity
		glm::vec3 relVel = velA - velB;

		// find out how much velocity each object had in the collision normal direction
		glm::vec3 colVector = colNormal * (glm::dot(relVel, colNormal));

		// calculate impulse force
		glm::vec3 impulse = colVector / (1.0f / massA + 1.0f / massB);			// TODO: May be wrong. double check theory

		// move spheres so that they're not overlapping
		glm::vec3 separate = iter->intersect.collisionVector;
		iter->objA->SetPosition(iter->objA->GetPosition() - (separate * (massB / (massA + massB))));
		iter->objB->SetPosition(iter->objB->GetPosition() + (separate * (massA / (massA + massB))));

		// apply that force to both objects (each in an opposite direction)
		iter->objA->SetVelocity(velA - (impulse * massB));
		iter->objB->SetVelocity(velB + (impulse * massA));
	}

}
