#pragma once

#include <Nest.h>

#include <geometry/Vector3.h>

#include "Camera3D.h"

class SandboxLayer : public Nest::Layer
{
public:
	SandboxLayer();

	void onAttach() override;
	void onDetach() override;
	void onUpdate(std::chrono::milliseconds timestep) override;
	void onEvent(Nest::Event &e) override;

private:
	Nest::OrthographicCamera m_camera;
	Camera3D m_camera3D;

	Nest::Ref<Nest::Shader> m_shader;
};