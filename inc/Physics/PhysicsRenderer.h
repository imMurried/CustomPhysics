#pragma once

#include <glm\vec4.hpp>

#include <map>

namespace Physics
{
	class Scene;
	class Object;

	class Renderer
	{
	public:

		struct RenderInfo
		{
			glm::vec4 color = glm::vec4(1, 1, 1, 1);
		};

		Renderer();
		virtual ~Renderer();

		virtual void Draw(Scene *scene);

		RenderInfo* GetRenderInfo(Object* obj);

	protected:
		std::map<Object*, RenderInfo> m_renderInfo;

	private:
		void RenderGizmosPhysics(Scene * scene);
	};

}

