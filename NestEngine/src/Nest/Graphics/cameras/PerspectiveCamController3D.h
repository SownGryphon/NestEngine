#pragma once

#include <chrono>

#include <geometry/Vector3.h>

#include "Nest/Events/Event.h"
#include "PerspectiveCamera3D.h"

namespace Nest
{
	class PerspectiveCamController3D : public PerspectiveCamera3D
	{
	public:
		PerspectiveCamController3D() = default;
		PerspectiveCamController3D(const PerspectiveCamera3D &cam, float moveSpeed, float turnSpeed) : PerspectiveCamera3D(cam), m_moveSpeed(moveSpeed), m_turnSpeed(turnSpeed) {}
		PerspectiveCamController3D(float FOV, float aspectRatio, float near, float far, chcl::Vector3<float> position, float pitch, float yaw, float moveSpeed, float turnSpeed);

		void onUpdate(std::chrono::milliseconds timestep);
		void onEvent(Event &e);

	private:
		float m_moveSpeed = 1.f, m_turnSpeed = 1.2f;
		bool m_mouseCaptured = false;
	};
}