#pragma once

#include <vector>

namespace Physics
{
	class Object;

	class Constraint
	{
	public:

		enum class Type
		{
			SPRING,
			JOINT,
		};

		Constraint(const std::vector<Object*> &objects, Type type);
		virtual ~Constraint();

		virtual void Update() = 0;

		const std::vector<Object*>& GetObjects() { return m_objects; }
		Type GetType() const { return m_type; }

	protected:
		std::vector<Object*> m_objects;
		Type m_type;
	private:
	};
}