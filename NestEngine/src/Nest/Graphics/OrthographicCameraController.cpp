#include "OrthographicCameraController.h"

#include <math.h>

#include "Nest/Core/Input.h"

namespace Nest
{
	Nest::OrthographicCameraController::OrthographicCameraController(float aspectRatio)
		: m_aspectRatio(aspectRatio), m_zoom(0.f),
		m_camera(-aspectRatio / getZoomFactor(), aspectRatio / getZoomFactor(), -1 / getZoomFactor(), 1 / getZoomFactor(), -1, 1)
	{
	}

	OrthographicCameraController::OrthographicCameraController(chcl::Rect viewArea)
		: m_aspectRatio(viewArea.size.x / viewArea.size.y), m_zoom(calculateZoom(2 * m_aspectRatio / viewArea.size.x)), m_position(viewArea.center()),
		m_camera(-m_aspectRatio / getZoomFactor(), m_aspectRatio / getZoomFactor(), -1 / getZoomFactor(), 1 / getZoomFactor(), -1, 1)
	{
		m_camera.setPosition(m_position);
	}

	void OrthographicCameraController::onUpdate()
	{
		float zoomFactor = getZoomFactor();

		if (Input::IsKeyPressed(NE_KEY_S))
		{
			m_position.y -= m_cameraTranslationSpeed / zoomFactor;
		}

		if (Input::IsKeyPressed(NE_KEY_W))
		{
			m_position.y += m_cameraTranslationSpeed / zoomFactor;
		}

		if (Input::IsKeyPressed(NE_KEY_A))
		{
			m_position.x -= m_cameraTranslationSpeed / zoomFactor;
		}

		if (Input::IsKeyPressed(NE_KEY_D))
		{
			m_position.x += m_cameraTranslationSpeed / zoomFactor;
		}

		m_camera.setPosition(m_position);
	}

	bool OrthographicCameraController::onEvent(Event & e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatchEvent<MouseScrolledEvent>(NE_BIND_EVENT_FN(OrthographicCameraController::onMouseScrollEvent));
		return false;
	}

	void OrthographicCameraController::capture(chcl::Rect area)
	{
		float zoomFactor = std::max(area.size.x / m_aspectRatio, area.size.y) / 2;
		setZoomFactor(1 / zoomFactor);
		m_position = area.center();
		updateCameraZoom();
	}

	void OrthographicCameraController::fit(chcl::Rect area)
	{
		float zoomFactor = std::min(area.size.x / m_aspectRatio, area.size.y) / 2;
		setZoomFactor(1 / zoomFactor);
		m_position = area.center();
		updateCameraZoom();
	}

	bool OrthographicCameraController::onMouseScrollEvent(MouseScrolledEvent &e)
	{
		m_zoom += e.getDY() * 0.2f;
		updateCameraZoom();
		return true;
	}

	float OrthographicCameraController::calculateZoom(float zoomFactor)
	{
		return std::log2f(zoomFactor);
	}

	float OrthographicCameraController::getZoomFactor() const
	{
		return std::powf(2, m_zoom);
	}

	void OrthographicCameraController::updateCameraZoom()
	{
		float zoomFactor = getZoomFactor();
		m_camera.setProjection(-m_aspectRatio / zoomFactor, m_aspectRatio / zoomFactor, -1 / zoomFactor, 1 / zoomFactor, -1.f, 1.f);
	}
}
