#pragma once

#include <geometry/Vector2.h>
#include <geometry/Rect.h>

#include "Nest/Events/Event.h"
#include "Nest/Events/MouseEvent.h"
#include "Nest/Graphics/OrthographicCamera.h"

namespace Nest
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio);
		OrthographicCameraController(chcl::Rect viewArea);

		void onUpdate();
		bool onEvent(Event &e);

		inline void setPosition(chcl::Vector2<> pos) { m_position = pos; }
		// Moves and zooms camera to fit the given area into view window
		void capture(chcl::Rect area);
		// Moves and zooms camera to capture as much of the given area as possible without capturing anything outside it
		void fit(chcl::Rect area);

		inline void setZoom(float zoom) { m_zoom = zoom; updateCameraZoom(); }
		inline void setZoomFactor(float zoomFactor) { m_zoom = calculateZoom(zoomFactor); updateCameraZoom(); }
		inline float getZoom() const { return m_zoom; }

		inline OrthographicCamera& getCamera() { return m_camera; }
		inline const OrthographicCamera& getCamera() const { return m_camera; }

	private:
		bool onMouseScrollEvent(MouseScrolledEvent &e);

		// Converts a zoom factor (2x, 3x, 0.5x etc.) to camera zoom amount
		static float calculateZoom(float zoomFactor);
		float getZoomFactor() const;
		void updateCameraZoom();

		float m_aspectRatio, m_zoom;
		chcl::Vector2<> m_position;
		OrthographicCamera m_camera;

		float m_cameraTranslationSpeed = 0.05f;
	};

}