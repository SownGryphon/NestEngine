#include "PerspectiveCamController3D.h"

#include "Nest/Core/Input.h"

#include <maths/Mat3.h>

Nest::PerspectiveCamController3D::PerspectiveCamController3D(float FOV, float aspectRatio, float near, float far, chcl::Vector3<float> position, float pitch, float yaw, float moveSpeed, float turnSpeed) :
	PerspectiveCamera3D(FOV, aspectRatio, near, far, position, pitch, yaw),
	m_moveSpeed(moveSpeed), m_turnSpeed(turnSpeed)
{}

void Nest::PerspectiveCamController3D::onUpdate(std::chrono::milliseconds timestep)
{
	if (Input::WasMouseButtonPressed(0))
	{
		m_mouseCaptured = true;
		Input::CaptureCursor(true);
	}

	if (Input::WasKeyPressed(NE_KEY_ESCAPE))
	{
		m_mouseCaptured = false;
		Input::CaptureCursor(false);
	}

	if (!m_mouseCaptured)
		return;

	chcl::Vector3<float> moveDir;

	if (Nest::Input::IsKeyDown(NE_KEY_W))
		moveDir.z -= 1.f;
	if (Nest::Input::IsKeyDown(NE_KEY_S))
		moveDir.z += 1.f;
	if (Nest::Input::IsKeyDown(NE_KEY_A))
		moveDir.x -= 1.f;
	if (Nest::Input::IsKeyDown(NE_KEY_D))
		moveDir.x += 1.f;
	if (Nest::Input::IsKeyDown(NE_KEY_SPACE))
		moveDir.y += 1.f;
	if (Nest::Input::IsKeyDown(NE_KEY_LEFT_SHIFT))
		moveDir.y -= 1.f;

	if (moveDir)
	{
		chcl::Mat3 rotationMat = chcl::Mat4::Resize<3, 3>(getRotationMat());
		m_pos += rotationMat * moveDir.normalized() * (float)timestep.count() * 0.001f * m_moveSpeed;
	}

	if (m_mouseCaptured)
	{
		chcl::Vector2<float> mouseMovement = Input::GetCursorMovement();
		m_pitch -= mouseMovement.y * m_turnSpeed * timestep.count() * 0.001f;
		m_yaw += mouseMovement.x * m_turnSpeed * timestep.count() * 0.001f;
	}

	m_FOV *= std::powf(1.05f, -Input::GetMouseScroll());

	recalculateMatrix();
}

void Nest::PerspectiveCamController3D::onEvent(Event & e)
{
}