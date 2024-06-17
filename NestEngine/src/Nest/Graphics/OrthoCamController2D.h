#pragma once

#include <geometry/Vector2.h>
#include <geometry/Rect.h>

#include "Nest/Events/Event.h"
#include "Nest/Events/MouseEvent.h"
#include "Nest/Graphics/OrthographicCamera2D.h"

namespace Nest
{
	class OrthoCamController2D
	{
	public:
		OrthoCamController2D(chcl::Vector2<float> pos, float aspectRatio);
		OrthoCamController2D(chcl::Rect viewArea);

		void onUpdate();
		bool onEvent(Event &e);

		inline void setPosition(chcl::Vector2<float> pos) { m_camera.setPos(pos); }
		inline void setZoom(float zoom) { m_zoom = zoom; updateCamera(); }

		inline chcl::Vector2<float> getPos() const { return m_camera.getPos(); }
		inline float getZoom() const { return m_zoom; }

		inline OrthographicCamera2D& getCamera() { return m_camera; }
		inline const OrthographicCamera2D& getCamera() const { return m_camera; }

	private:
		bool onMouseScrollEvent(MouseScrolledEvent &e);

		void updateCamera();

		float m_aspectRatio, m_zoom;
		OrthographicCamera2D m_camera;

		float m_cameraTranslationSpeed = 0.05f;
	};

}