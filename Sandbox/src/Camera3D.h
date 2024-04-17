#pragma once

#include <chrono>

#include <geometry/Vector3.h>
#include <maths/SquareMatrix.h>

class Camera3D
{
public:
	Camera3D(chcl::Vector3<float> position, float yaw, float pitch);

	void update(std::chrono::milliseconds timestep);

	inline chcl::Vector3<float> getPosition() const { return m_position; }

	chcl::Mat4 getPositionTransform() const;
	chcl::Mat3 getRotationTransform() const;

private:
	chcl::Vector3<float> m_position;
	float m_yaw, m_pitch;

	static const float s_moveSpeed, s_turnSpeed;
};