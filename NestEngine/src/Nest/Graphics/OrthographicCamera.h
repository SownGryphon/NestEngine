#pragma once

#include <geometry/Vector2.h>
#include <maths/SquareMatrix.h>

namespace Nest
{
	class OrthographicCamera
	{
	public:
		const chcl::Mat4& getViewProjectionMatrix() const { return m_VPM; }

	protected:
		// View projection matrix
		chcl::Mat4 m_VPM;
	};
}