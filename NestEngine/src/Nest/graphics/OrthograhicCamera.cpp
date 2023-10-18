#include "OrthograhicCamera.h"

namespace Nest
{
	OrthograhicCamera::OrthograhicCamera(float left, float right, float bottom, float top, float back, float front)
		: m_ViewProjectionMatrix(chcl::Mat4::Ortho(left, right, bottom, top, back, front))
	{
	}

	void OrthograhicCamera::setProjection(float left, float right, float bottom, float top, float back, float front)
	{
		m_ViewProjectionMatrix = chcl::Mat4::Ortho(left, right, bottom, top, back, front);
	}
}