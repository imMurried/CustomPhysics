#include "Physics_Walkthrough_App.h"
#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "PhysicsRenderer.h"
#include "AABBCollider.h"
#include "SphereCollider.h"
#include "Spring.h"

#include "Gizmos.h"
#include "Input.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

Physics_Walkthrough_App::Physics_Walkthrough_App()
{

}

Physics_Walkthrough_App::~Physics_Walkthrough_App()
{

}

bool Physics_Walkthrough_App::startup()
{
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	aie::Gizmos::create(1000000, 1000000, 1000000, 1000000);

	// create simple camera transforms
	m_camera = new Camera();
	m_camera->SetProjection(glm::radians(45.0f), (float)getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);
	m_camera->SetPosition(glm::vec3(10, 10, 10));
	m_camera->Lookat(glm::vec3(0, 0, 0));

	// Create Scene
	m_scene = new Physics::Scene();

	// Create debug Physics Renderer
	m_renderer = new Physics::Renderer();

	//// spring test

	//Physics::Object * left = new Physics::Object();
	//left->SetPosition(glm::vec3(-2, 1, 0));
	//left->SetCollider(new Physics::SphereCollider(1.0f));
	//m_scene->AttachObject(left);

	//Physics::Object * right = new Physics::Object();
	//right->SetPosition(glm::vec3(2, 1, 0));
	//right->SetCollider(new Physics::SphereCollider(1.0f));
	//m_scene->AttachObject(right);

	//Physics::Object * top = new Physics::Object();
	//top->SetPosition(glm::vec3(0, 3, 0));
	//top->SetCollider(new Physics::SphereCollider(1.0f));
	//m_scene->AttachObject(top);

	//Physics::Object * back = new Physics::Object();
	//back->SetPosition(glm::vec3(0, 1, 2));
	//back->SetCollider(new Physics::SphereCollider(1.0f));
	//m_scene->AttachObject(back);

	//Physics::Spring* spring = new Physics::Spring(left, right, 7.0f, 200.0f, 0.5f);
	//m_scene->AttachConstraint(spring);

	//Physics::Spring* spring1 = new Physics::Spring(left, back, 7.0f, 200.0f, 0.5f);
	//m_scene->AttachConstraint(spring1);

	//Physics::Spring* spring2 = new Physics::Spring(left, top, 7.0f, 200.0f, 0.5f);
	//m_scene->AttachConstraint(spring2);

	//Physics::Spring* spring3 = new Physics::Spring(right, back, 7.0f, 200.0f, 0.5f);
	//m_scene->AttachConstraint(spring3);

	//Physics::Spring* spring4 = new Physics::Spring(right, top, 7.0f, 200.0f, 0.5f);
	//m_scene->AttachConstraint(spring4);

	//Physics::Spring* spring5 = new Physics::Spring(back, top, 7.0f, 200.0f, 0.5f);
	//m_scene->AttachConstraint(spring5);

	const int maxX = 2;
	const int maxY = 2;
	const int maxZ = 2;

	double length = 1;
	double diagonalLength = sqrt((length*length) + (length*length));
	double innerDiagonal = sqrt((diagonalLength*diagonalLength) + (length*length));
	double stiffness = 1000;
	double dampening = 0.5f;

	Physics::Object* blob[maxX][maxY][maxZ];
	// Add Objects to scene
	for (int x = 0; x < maxX; x++)
	{
		for (int y = 0; y < maxY; y++)
		{
			for (int z = 0; z < maxZ; z++)
			{
				blob[x][y][z] = new Physics::Object();
				blob[x][y][z]->SetPosition(glm::vec3(x * length, y * length, z * length));
				blob[x][y][z]->SetCollider(new Physics::SphereCollider(0.3f));
				m_scene->AttachObject(blob[x][y][z]);

				m_renderer->GetRenderInfo(blob[x][y][z])->color = glm::vec4{
					rand() % 255 / 255.0f,			//r
					rand() % 255 / 255.0f,			//g
					rand() % 255 / 255.0f,			//b
					0.5f							//a
				};
			}
		}
	}

	//Stress
	const int xx = 12;
	const int yy = 12;
	const int zz = 12;
	Physics::Object* stress[xx][yy][zz];
	// Add Objects to scene
	for (int x = 0; x < xx; x++)
	{
		for (int y = 0; y < yy; y++)
		{
			for (int z = 0; z < zz; z++)
			{
				stress[x][y][z] = new Physics::Object();
				stress[x][y][z]->SetPosition(glm::vec3(x, y + 5, z));
				stress[x][y][z]->SetCollider(new Physics::SphereCollider(0.3f));
				m_scene->AttachObject(stress[x][y][z]);

				m_renderer->GetRenderInfo(stress[x][y][z])->color = glm::vec4{
					rand() % 255 / 255.0f,			//r
					rand() % 255 / 255.0f,			//g
					rand() % 255 / 255.0f,			//b
					0.5f							//a
				};
			}
		}
	}

	for (int x = 0; x < maxX; x++)
	{
		for (int y = 0; y < maxY; y++)
		{
			for (int z = 0; z < maxZ; z++)
			{
				// in here, we add constraints from each node to each other nodes
				// around it (above, below, left, right, forwards, back and diagonal
				if (x < maxX - 1)
				{
					Physics::Spring* s = new Physics::Spring(blob[x][y][z], blob[x + 1][y][z], length, stiffness, dampening);
					m_scene->AttachConstraint(s);
				}

				if (y < maxY - 1)
				{
					Physics::Spring* s1 = new Physics::Spring(blob[x][y][z], blob[x][y + 1][z], length, stiffness, dampening);
					m_scene->AttachConstraint(s1);
				}

				if (z < maxZ - 1)
				{
					Physics::Spring* s2 = new Physics::Spring(blob[x][y][z], blob[x][y][z + 1], length, stiffness, dampening);
					m_scene->AttachConstraint(s2);
				}

				

				if (x < maxX - 1 && y < maxY - 1)
				{
				Physics::Spring* spring6 = new Physics::Spring(blob[x][y][z], blob[x + 1][y + 1][z], diagonalLength, stiffness, dampening);
				m_scene->AttachConstraint(spring6);
				}

				if (x < maxX - 1 && z < maxZ - 1)
				{
				Physics::Spring* spring8 = new Physics::Spring(blob[x][y][z], blob[x + 1][y][z + 1], diagonalLength, stiffness, dampening);
				m_scene->AttachConstraint(spring8);
				}

				if (z < maxZ - 1 && y < maxY - 1)
				{
				Physics::Spring* spring0 = new Physics::Spring(blob[x][y][z], blob[x][y + 1][z + 1], diagonalLength, stiffness, dampening);
				m_scene->AttachConstraint(spring0);
				}


				if (z > 0 && y < maxY - 1)
				{
					Physics::Spring* s7 = new Physics::Spring(blob[x][y][z], blob[x][y + 1][z - 1], diagonalLength, stiffness, dampening);
					m_scene->AttachConstraint(s7);
				}

				if (x > 0 && y < maxY - 1)
				{
					Physics::Spring* s8 = new Physics::Spring(blob[x][y][z], blob[x - 1][y + 1][z], diagonalLength, stiffness, dampening);
					m_scene->AttachConstraint(s8);
				}

				if (z < maxZ - 1 && x > 0)
				{
					Physics::Spring* s9 = new Physics::Spring(blob[x][y][z], blob[x - 1][y][z + 1], diagonalLength, stiffness, dampening);
					m_scene->AttachConstraint(s9);
				}



				if (x < maxX - 1 && y < maxY - 1 && z < maxZ - 1)
				{
					Physics::Spring* ss = new Physics::Spring(blob[x][y][z], blob[x + 1][y + 1][z + 1], innerDiagonal, stiffness, dampening);
					m_scene->AttachConstraint(ss);
				}

				if (x < maxX - 1 && y > 0 && z < maxZ - 1)
				{
					Physics::Spring* ss1 = new Physics::Spring(blob[x][y][z], blob[x + 1][y - 1][z + 1], innerDiagonal, stiffness, dampening);
					m_scene->AttachConstraint(ss1);
				}

				if (x > 0 && y < maxY - 1 && z < maxZ - 1)
				{
					Physics::Spring* ss2 = new Physics::Spring(blob[x][y][z], blob[x - 1][y + 1][z + 1], innerDiagonal, stiffness, dampening);
					m_scene->AttachConstraint(ss2);
				}

				if (x > 0 && y > 0 && z < maxZ - 1)
				{
					Physics::Spring* ss3 = new Physics::Spring(blob[x][y][z], blob[x - 1][y - 1][z + 1], innerDiagonal, stiffness, dampening);
					m_scene->AttachConstraint(ss3);
				}
			}
		}
	}

	Physics::Object *obj = new Physics::Object();
	obj->SetPosition(glm::vec3(5, 0, 5));
	obj->SetCollider(new Physics::AABBCollider());
	m_scene->AttachObject(obj);
	

	//for (int i = 0; i < 10; i++)
	//{
	//	Physics::Object *obj = new Physics::Object();
	//	obj->SetPosition(glm::vec3(i * 1.0f, 3.0f * i, 0.0f));
	//	obj->SetCollider(new Physics::SphereCollider(0.8f));
	//	m_scene->AttachObject(obj);

	//	m_renderer->GetRenderInfo(obj)->color = glm::vec4(
	//		rand() % 255 / 255.0f, // r
	//		rand() % 255 / 255.0f, // g
	//		rand() % 255 / 255.0f, // b
	//		1.0f				   // alpha
	//	);
	//}

	return true;
}

void Physics_Walkthrough_App::shutdown()
{
	aie::Gizmos::destroy();
}

void Physics_Walkthrough_App::update(float deltaTime)
{
	m_camera->Update(deltaTime);
	
	timer += deltaTime;
	if (timer >= shutdownTime) quit();

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
	float force = 5.0f;

	if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT))
	{
		Physics::Object *obj = new Physics::Object();
		obj->SetPosition(m_camera->GetPosition());
		obj->SetVelocity(m_camera->GetFront() * 10.0f);
		obj->SetCollider(new Physics::AABBCollider());
		obj->SetMass(10.0f);
		m_renderer->GetRenderInfo(obj)->color = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
		m_scene->AttachObject(obj);
	}

	// apply gravity to scene
	m_scene->ApplyGlobalForce(glm::vec3(0, -9.8f, 0));

	m_scene->Update(1 / 60.0f);
}

void Physics_Walkthrough_App::draw()
{
	// wipe the screen to the background colour
	clearScreen();

	// wipe the gizmos clean for this frame
	aie::Gizmos::clear();
	RenderGizmosGrid();

	// Create Gizmos for all the objects
	m_renderer->Draw(m_scene);

	aie::Gizmos::draw(m_camera->GetProjectionView());
}

void Physics_Walkthrough_App::RenderGizmosGrid()
{
	// draw a simple grid with gizmos
	glm::vec4 white(1);
	glm::vec4 black(0, 0, 0, 1);

	for (int i = 0; i < 21; ++i)
	{
		aie::Gizmos::addLine(glm::vec3(-10 + i, 0, 10), glm::vec3(-10 + i, 0, -10), i == 10 ? white : black);
		aie::Gizmos::addLine(glm::vec3(10, 0, -10 + i), glm::vec3(-10, 0, -10 + i), i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	aie::Gizmos::addTransform(glm::mat4());
}
