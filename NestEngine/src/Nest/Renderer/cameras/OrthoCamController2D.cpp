#include "OrthoCamController2D.h"

#include <math.h>

#include "Nest/Core/Input.h"

namespace Nest
{
	Nest::OrthoCamController2D::OrthoCamController2D(chcl::Vector2<float> pos, float aspectRatio) :
		OrthographicCamera2D(pos, { aspectRatio, 1.f }),
		m_aspectRatio(aspectRatio), m_zoom(0.f)
	{}

	OrthoCamController2D::OrthoCamController2D(chcl::Rect viewArea) :
		OrthographicCamera2D(viewArea),
		m_aspectRatio(viewArea.m_size.x / viewArea.m_size.y), m_zoom(2 * m_aspectRatio / viewArea.m_size.x)
	{}

	void OrthoCamController2D::onUpdate()
	{
		if (Input::IsKeyDown(NE_KEY_S))
		{
			m_pos.y -= m_cameraTranslationSpeed * m_zoom;
		}

		if (Input::IsKeyDown(NE_KEY_W))
		{
			m_pos.y += m_cameraTranslationSpeed * m_zoom;;
		}

		if (Input::IsKeyDown(NE_KEY_A))
		{
			m_pos.x -= m_cameraTranslationSpeed * m_zoom;;
		}

		if (Input::IsKeyDown(NE_KEY_D))
		{
			m_pos.x += m_cameraTranslationSpeed * m_zoom;;
		}

		recalculateViewMatrix();
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
		setSize(chcl::Vector2<float>(m_aspectRatio, 1.f) * m_zoom);
	}
}
