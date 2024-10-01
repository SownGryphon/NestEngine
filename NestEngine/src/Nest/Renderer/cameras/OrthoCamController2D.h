#pragma once

#include <geometry/Vector2.h>
#include <geometry/Rect.h>

#include "Nest/Events/Event.h"
#include "Nest/Events/MouseEvent.h"
#include "Nest/renderer/cameras/OrthographicCamera2D.h"

namespace Nest
{
	class OrthoCamController2D : public OrthographicCamera2D
	{
	public:
		OrthoCamController2D(chcl::Vector2<float> pos, float aspectRatio);
		OrthoCamController2D(chcl::Rect viewArea);

		void onUpdate();
		bool onEvent(Event &e);

		inline void setZoom(float zoom) { m_zoom = zoom; updateCamera(); }

		inline float getZoom() const { return m_zoom; }

	private:
		bool onMouseScrollEvent(MouseScrolledEvent &e);

		void updateCamera();

		float m_aspectRatio, m_zoom;

		float m_cameraTranslationSpeed = 0.05f;
	};

}