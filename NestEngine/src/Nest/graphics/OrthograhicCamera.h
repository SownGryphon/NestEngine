#pragma once

#include <maths/SquareMatrix.h>

namespace Nest
{
	class OrthograhicCamera
	{
	public:
		OrthograhicCamera(float left, float right, float bottom, float top, float back, float front);

		void setProjection(float left, float right, float bottom, float top, float back, float front);

		inline const chcl::Mat4& getViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		chcl::Mat4 m_ViewProjectionMatrix;
	};
}