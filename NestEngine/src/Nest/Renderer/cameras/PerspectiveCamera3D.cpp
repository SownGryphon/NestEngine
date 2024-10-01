#include "PerspectiveCamera3D.h"

Nest::PerspectiveCamera3D::PerspectiveCamera3D(float FOV, float aspectRatio, float near, float far, chcl::Vector3<float> pos, float pitch, float yaw) :
	m_FOV(FOV), m_aspect(aspectRatio), m_near(near), m_far(far),
	m_pos(pos), m_pitch(pitch), m_yaw(yaw)
{
	recalculateMatrix();
}

void Nest::PerspectiveCamera3D::recalculateMatrix()
{
	float tanHalfFOV = std::tanf(m_FOV / 2.f);

	chcl::Mat4 perspectiveTransform = {
		1.f / tanHalfFOV, 0, 0, 0,
		0, m_aspect / tanHalfFOV, 0, 0,
		0, 0, -(m_far + m_near) / (m_far - m_near), -2.f * m_far * m_near / (m_far - m_near),
		0, 0, -1.f, 0
	};

	m_VPM = perspectiveTransform * chcl::Mat4::RotationYZ(-m_pitch) * chcl::Mat4::RotationXZ(-m_yaw) * chcl::Mat4::Translation3D(-m_pos);
}

chcl::Mat4 Nest::PerspectiveCamera3D::getRotationMat() const
{
	return chcl::Mat4::RotationXZ(m_yaw) * chcl::Mat4::RotationYZ(m_pitch);
}

chcl::Mat4 Nest::PerspectiveCamera3D::getRotationMatInv() const
{
	return chcl::Mat4::RotationYZ(-m_pitch) * chcl::Mat4::RotationXZ(-m_yaw);
}