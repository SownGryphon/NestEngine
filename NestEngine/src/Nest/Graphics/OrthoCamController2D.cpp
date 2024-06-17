#include "OrthoCamController2D.h"

#include <math.h>

#include "Nest/Core/Input.h"

namespace Nest
{
	Nest::OrthoCamController2D::OrthoCamController2D(chcl::Vector2<float> pos, float aspectRatio) :
		m_aspectRatio(aspectRatio), m_zoom(0.f),
		m_camera(pos, { aspectRatio, 1.f })
	{}

	OrthoCamController2D::OrthoCamController2D(chcl::Rect viewArea) :
		m_aspectRatio(viewArea.m_size.x / viewArea.m_size.y), m_zoom(2 * m_aspectRatio / viewArea.m_size.x),
		m_camera(viewArea)
	{}

	void OrthoCamController2D::onUpdate()
	{
		chcl::Vector2<float> currentPos = m_camera.getPos();

		if (Input::IsKeyPressed(NE_KEY_S))
		{
			currentPos.y -= m_cameraTranslationSpeed * m_zoom;
		}

		if (Input::IsKeyPressed(NE_KEY_W))
		{
			currentPos.y += m_cameraTranslationSpeed * m_zoom;;
		}

		if (Input::IsKeyPressed(NE_KEY_A))
		{
			currentPos.x -= m_cameraTranslationSpeed * m_zoom;;
		}

		if (Input::IsKeyPressed(NE_KEY_D))
		{
			currentPos.x += m_cameraTranslationSpeed * m_zoom;;
		}

		m_camera.setPos(currentPos);
	}

	bool OrthoCamController2D::onEvent(Event &e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatchEvent<MouseScrolledEvent>(NE_BIND_EVENT_FN(OrthoCamController2D::onMouseScrollEvent));
		return false;
	}

	bool OrthoCamController2D::onMouseScrollEvent(MouseScrolledEvent &e)
	{
		m_zoom *= std::powf(2.f, e.getDY() * 0.2f);
		updateCamera();
		return true;
	}

	void OrthoCamController2D::updateCamera()
	{
		m_camera.setSize(chcl::Vector2<float>(m_aspectRatio, 1.f) * m_zoom);
	}
}
