#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera
{
public:

	Camera();
	virtual ~Camera();

	virtual void Update(float deltaTime);

	void SetPosition(const glm::vec3 &pos);
	void SetYawPitchRoll(float yaw, float pitch, float roll);
	void SetProjection(float fov, float aspect, float near, float far);

	glm::vec3& GetPosition();

	float GetYaw();
	float GetPitch();
	float GetRoll();

	glm::vec3& GetFront();

	glm::mat4& GetView();
	glm::mat4& GetProjection();
	glm::mat4 GetProjectionView();

	void Lookat(glm::vec3 target);

	void SetMovementKeys(int forwardKey, int backKey, int leftKey, int rightKey);
	void SetMouseLookButton(int mouseButton);

protected:

	void CalculateFront();

protected:

	glm::vec3 m_cameraPosition = glm::vec3(0, 2, -2);
	glm::vec3 m_cameraFront = glm::vec3(0, 0, 1);
	glm::vec3 m_cameraUp = glm::vec3(0, 1, 0);

	float m_cameraYaw = 0.0f;
	float m_cameraPitch = 0.0f;
	float m_cameraRoll = 0.0f;

	float m_cameraMoveSpeed = 10.0f;

	int m_lastMouseXPos, m_lastMouseYPos;

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	// movement keys
	int m_forwardKey;	// defaults to W
	int m_backKey;		// defaults to S
	int m_leftKey;		// defaults to A
	int m_rightKey;		// defaults to D

	int m_mouseLookButton; // defaults to right mouse button

private:
};
