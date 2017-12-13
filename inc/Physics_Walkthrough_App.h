#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>

class Camera;

namespace Physics
{
	class Scene;
	class Renderer;
}

class Physics_Walkthrough_App : public aie::Application {
public:

	Physics_Walkthrough_App();
	virtual ~Physics_Walkthrough_App();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();


protected:

	Physics::Scene* m_scene = nullptr;
	Physics::Renderer* m_renderer = nullptr;

	// camera transforms
	Camera *m_camera = nullptr;

private:
	void RenderGizmosGrid();
	float shutdownTime = 60.0f;
	float timer = 0.0f;
};