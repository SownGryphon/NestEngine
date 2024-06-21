#include "OrthographicCamera2D.h"

#include <maths/Common.h>

namespace Nest
{
	OrthographicCamera2D::OrthographicCamera2D(chcl::Vector2<float> pos, chcl::Vector2<float> size, float rotation) :
		m_pos(pos), m_size(size), m_rotation(rotation)
	{
		recalculateViewMatrix();
	}

	OrthographicCamera2D::OrthographicCamera2D(chcl::Rect viewArea) :
		m_pos(viewArea.m_center), m_size(viewArea.m_size), m_rotation(viewArea.m_rotation)
	{
		recalculateViewMatrix();
	}

	void OrthographicCamera2D::recalculateViewMatrix()
	{
		m_VPM = chcl::Mat4::RotationXY(-chcl::toRadians(m_rotation)) * chcl::Mat4::Scale3D(2.f / m_size) * chcl::Mat4::Translation3D(-m_pos);
	}

	void OrthographicCamera2D::capture(chcl::Rect area, float aspectRatio)
	{
		float areaAspectRatio = area.m_size.x / area.m_size.y;
		m_pos = area.m_center;
		m_size = area.m_size;
		m_rotation = area.m_rotation;
		if (areaAspectRatio > aspectRatio)
			m_size.x *= aspectRatio / areaAspectRatio;
		else
			m_size.y *= aspectRatio / areaAspectRatio;
		recalculateViewMatrix();
	}

	void OrthographicCamera2D::fit(chcl::Rect area, float aspectRatio)
	{
		float areaAspectRatio = area.m_size.x / area.m_size.y;
		m_pos = area.m_center;
		m_size = area.m_size;
		m_rotation = area.m_rotation;
		if (areaAspectRatio > aspectRatio)
			m_size.y *= aspectRatio / areaAspectRatio;
		else
			m_size.x *= aspectRatio / areaAspectRatio;
		recalculateViewMatrix();
	}
}