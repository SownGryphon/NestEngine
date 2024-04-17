#pragma once

#include <geometry/Vector2.h>
#include <maths/SquareMatrix.h>

namespace Nest
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float back, float front);

		void setProjection(float left, float right, float bottom, float top, float back, float front);

		void setPosition(chcl::Vector2<> position);
		inline chcl::Vector2<> getPosition() { return m_position; }

		inline const chcl::Mat4& getViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		chcl::Mat4 m_ProjectionMatrix;
		chcl::Mat4 m_ViewProjectionMatrix;

		chcl::Vector2<> m_position;

		void recalculateViewMatrix();
	};
}