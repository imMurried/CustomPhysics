#pragma once
#include "Constraint.h"

namespace Physics
{
	class Spring :	public Constraint
	{
	public:
		Spring(Object* objA, Object* objB);
		Spring(Object* objA, Object* objB, float length, float stiffness, float dampening);
		virtual ~Spring();

		void Update();
	protected:
		float m_length;
		float m_stiffness;
		float m_dampening;
	private:
	};
}