#include "Camera.h"
#include <Input.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

Camera::Camera()
{
	m_forwardKey	= aie::INPUT_KEY_W;
	m_backKey		= aie::INPUT_KEY_S;
	m_leftKey		= aie::INPUT_KEY_A;
	m_rightKey		= aie::INPUT_KEY_D;

	m_mouseLookButton = aie::INPUT_MOUSE_BUTTON_RIGHT;
}
Camera::~Camera()
{

}

void Camera::Update(float deltaTime)
{
	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(m_forwardKey))
	{
		m_cameraPosition += m_cameraFront * m_cameraMoveSpeed * deltaTime;
	}

	if (input->isKeyDown(m_backKey))
	{
		m_cameraPosition -= m_cameraFront * m_cameraMoveSpeed * deltaTime;
	}

	if (input->isKeyDown(m_leftKey))
	{
		glm::vec3 dir = glm::normalize(glm::cross(m_cameraFront, m_cameraUp));
		m_cameraPosition -= dir * m_cameraMoveSpeed * deltaTime;
	}

	if (input->isKeyDown(m_rightKey))
	{
		glm::vec3 dir = glm::normalize(glm::cross(m_cameraFront, m_cameraUp));
		m_cameraPosition += dir * m_cameraMoveSpeed * deltaTime;
	}

	if (input->isMouseButtonDown(m_mouseLookButton) )
	{
		int mouseXPos, mouseYPos;
		input->getMouseXY(&mouseXPos, &mouseYPos);

		float deltaMouseX = mouseXPos - m_lastMouseXPos;
		float deltaMouseY = mouseYPos - m_lastMouseYPos;

		m_cameraYaw += deltaMouseX/2;
		m_cameraPitch += deltaMouseY/2;

		if (m_cameraPitch > 85.0f) m_cameraPitch = 85.0f;
		if (m_cameraPitch <-85.0f) m_cameraPitch = -85.0f;

		CalculateFront();
	}

	m_viewMatrix = glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraFront, m_cameraUp);

	// record the current mouse position for use next frame.
	input->getMouseXY(&m_lastMouseXPos, &m_lastMouseYPos);

}

void Camera::CalculateFront()
{
	glm::vec3 front;
	front.x = glm::cos(glm::radians(m_cameraYaw)) * glm::cos(glm::radians(m_cameraPitch));
	front.y = glm::sin(glm::radians(m_cameraPitch));
	front.z = glm::sin(glm::radians(m_cameraYaw)) * glm::cos(glm::radians(m_cameraPitch));
	m_cameraFront = glm::normalize(front);
}

void Camera::SetProjection(float fov, float aspect, float near, float far)
{
	m_projectionMatrix = glm::perspective(fov, aspect, near, far);
}

void Camera::SetPosition(const glm::vec3& pos)
{
	m_cameraPosition = pos;
}
void Camera::SetYawPitchRoll(float yaw, float pitch, float roll)
{
	m_cameraYaw = yaw;
	m_cameraPitch = pitch;
	m_cameraRoll = roll;

	CalculateFront();
}

glm::vec3& Camera::GetPosition()
{
	return m_cameraPosition;
}

float Camera::GetYaw()
{
	return m_cameraYaw;
}
float Camera::GetPitch()
{
	return m_cameraPitch;
}
float Camera::GetRoll()
{
	return m_cameraRoll;
}

glm::vec3 & Camera::GetFront()
{
	return m_cameraFront;
}

glm::mat4& Camera::GetView()
{
	return m_viewMatrix;
}
glm::mat4& Camera::GetProjection()
{
	return m_projectionMatrix;
}
glm::mat4 Camera::GetProjectionView()
{
	return m_projectionMatrix * m_viewMatrix;
}
void Camera::Lookat(glm::vec3 target)
{
	glm::vec3 dir = glm::normalize(target - m_cameraPosition);
	m_cameraPitch = glm::degrees(glm::asin(dir.y));
	m_cameraYaw = glm::degrees(atan2(dir.y, dir.x));

	CalculateFront();
}

void Camera::SetMovementKeys(int forwardKey, int backKey, int leftKey, int rightKey)
{
	m_forwardKey = forwardKey;
	m_backKey = backKey;
	m_leftKey = leftKey;
	m_rightKey = rightKey;
}

void Camera::SetMouseLookButton(int mouseButton)
{
	m_mouseLookButton = mouseButton;
}