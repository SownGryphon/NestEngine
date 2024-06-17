#pragma once

#include <geometry/Vector2.h>
#include <geometry/Rect.h>
#include <maths/SquareMatrix.h>

namespace Nest
{
	class OrthographicCamera2D
	{
	public:
		OrthographicCamera2D(chcl::Vector2<float> pos, chcl::Vector2<float> size, float rotation = 0.f);
		OrthographicCamera2D(chcl::Rect viewArea);
		OrthographicCamera2D(const OrthographicCamera2D &camera) = default;

		// Moves and zooms camera to fit the given area into view window
		void capture(chcl::Rect area, float aspectRatio);
		// Moves and zooms camera to capture as much of the given area as possible without capturing anything outside it
		void fit(chcl::Rect area, float aspectRatio);

		inline void setPos(chcl::Vector2<float> pos) { m_pos = pos; recalculateViewMatrix(); }
		inline void setSize(chcl::Vector2<float> size) { m_size = size; recalculateViewMatrix(); }
		inline void setRotation(float degrees) { m_rotation = degrees; recalculateViewMatrix(); }

		inline chcl::Vector2<float> getPos() const { return m_pos; }
		inline chcl::Vector2<float> getSize() const { return m_size; }
		inline float getRotation() const { return m_rotation; }

		inline const chcl::Mat4& getVPM() const { return m_VPM; }

		OrthographicCamera2D& operator=(const OrthographicCamera2D &camera) = default;
		OrthographicCamera2D& operator=(OrthographicCamera2D &&camera) = default;

	private:
		chcl::Vector2<float> m_pos, m_size;
		float m_rotation;
		chcl::Mat4 m_VPM;

		void recalculateViewMatrix();
	};
}