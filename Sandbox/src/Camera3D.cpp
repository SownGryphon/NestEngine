#include "Camera3D.h"

#include <Nest.h>

const float Camera3D::s_moveSpeed = 2.7f;
const float Camera3D::s_turnSpeed = 1.2f;

Camera3D::Camera3D(chcl::Vector3<float> position, float yaw, float pitch) :
	m_position(position), m_yaw(yaw), m_pitch(pitch) {}

void Camera3D::update(std::chrono::milliseconds timestep)
{
	if (Nest::Input::IsKeyPressed(NE_KEY_UP))
		m_pitch += s_turnSpeed * float(timestep.count()) / 1000;
	if (Nest::Input::IsKeyPressed(NE_KEY_DOWN))
		m_pitch -= s_turnSpeed * float(timestep.count()) / 1000;
	if (Nest::Input::IsKeyPressed(NE_KEY_LEFT))
		m_yaw += s_turnSpeed * float(timestep.count()) / 1000;
	if (Nest::Input::IsKeyPressed(NE_KEY_RIGHT))
		m_yaw -= s_turnSpeed * float(timestep.count()) / 1000;

	chcl::Vector3<> moveDir = chcl::Vector3<>(0.f);
	if (Nest::Input::IsKeyPressed(NE_KEY_W))
		moveDir.y += 1;
	if (Nest::Input::IsKeyPressed(NE_KEY_S))
		moveDir.y -= 1;
	if (Nest::Input::IsKeyPressed(NE_KEY_A))
		moveDir.x -= 1;
	if (Nest::Input::IsKeyPressed(NE_KEY_D))
		moveDir.x += 1;
	if (Nest::Input::IsKeyPressed(NE_KEY_SPACE))
		moveDir.z += 1;
	if (Nest::Input::IsKeyPressed(NE_KEY_LEFT_SHIFT))
		moveDir.z -= 1;

	m_position += getRotationTransform() * s_moveSpeed * moveDir * float(timestep.count()) / 1000;
}

chcl::Mat4 Camera3D::getPositionTransform() const
{
	return chcl::Mat4::Translation(m_position.x, m_position.y, m_position.z);
}

chcl::Mat3 Camera3D::getRotationTransform() const
{
	return chcl::Mat3::Rotation(m_pitch, m_yaw);
}