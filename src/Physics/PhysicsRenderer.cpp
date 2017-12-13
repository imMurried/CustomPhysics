#include "PhysicsRenderer.h"
#include "PhysicsObject.h"
#include "PhysicsScene.h"
#include "Collider.h"
#include "AABBCollider.h"
#include "SphereCollider.h"
#include "Constraint.h"

#include <vector>

#include <glm\vec4.hpp>

#include <Gizmos.h>

using namespace Physics;

Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Physics::Renderer::Draw(Scene * scene)
{
	RenderGizmosPhysics(scene);

}

void Renderer::RenderGizmosPhysics(Scene * scene)
{
	const std::vector<Physics::Object*> & objects = scene->GetObjects();
	for (auto iter = objects.begin(); iter != objects.end(); iter++)
	{
		Physics::Object *obj = *iter;
		RenderInfo *info = GetRenderInfo(obj);

		glm::vec4 renderColor = info->color;

		if (scene->isObjectColliding(obj))
		{
			renderColor = glm::vec4(1, 0, 0, 1);
		}

		switch (obj->GetCollider()->GetType())
		{
		case Collider::Type::AABB:
			aie::Gizmos::addAABB(obj->GetPosition(), ((AABBCollider*)obj->GetCollider())->GetExtents(), renderColor);
			break;
		case Collider::Type::SPHERE: // Draw the collider's actual size sphere
			aie::Gizmos::addSphere(obj->GetPosition(), ((SphereCollider*)obj->GetCollider())->GetRadius(), 10, 10, renderColor);
			break;
		case Collider::Type::NONE: // No collider, just show a small sphere
			aie::Gizmos::addSphere(obj->GetPosition(), 0.2f, 10, 10, info->color);
			break;
		}
		
	}

	const std::vector<Physics::Constraint*> & cons = scene->GetConstraints();
	for (auto iter = cons.begin(); iter != cons.end(); iter++)
	{
		switch ((*iter)->GetType())
		{
		case Constraint::Type::SPRING:
			aie::Gizmos::addLine((*iter)->GetObjects()[0]->GetPosition(), (*iter)->GetObjects()[1]->GetPosition(), glm::vec4(0, 1, 1, 1));
			break;
		}
	}

}

Renderer::RenderInfo * Physics::Renderer::GetRenderInfo(Object * obj)
{
	return &m_renderInfo[obj];
}
