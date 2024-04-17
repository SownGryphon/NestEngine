#include "OrthographicCamera.h"

namespace Nest
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float back, float front)
		: m_ProjectionMatrix(chcl::Mat4::Ortho(left, right, bottom, top, back, front))
	{
		recalculateViewMatrix();
	}

	void OrthographicCamera::setProjection(float left, float right, float bottom, float top, float back, float front)
	{
		m_ProjectionMatrix = chcl::Mat4::Ortho(left, right, bottom, top, back, front);
		recalculateViewMatrix();
	}

	void OrthographicCamera::setPosition(chcl::Vector2<> position)
	{
		m_position = position;
		recalculateViewMatrix();
	}

	void OrthographicCamera::recalculateViewMatrix()
	{
		chcl::Mat4 transform = chcl::Mat4::Translation(-m_position.x, -m_position.y, 0);

		m_ViewProjectionMatrix = m_ProjectionMatrix * transform;
	}
}