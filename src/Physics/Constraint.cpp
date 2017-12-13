#include "Constraint.h"

using namespace Physics;

Physics::Constraint::Constraint(const std::vector<Object*> &objects, Type type) :
	m_objects(objects), m_type(type)
{
	
}

Constraint::~Constraint()
{
}

void Physics::Constraint::Update()
{
}
