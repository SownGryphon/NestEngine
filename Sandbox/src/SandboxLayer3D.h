#pragma once

#include "Nest.h"

class SandboxLayer3D : public Nest::Layer
{
public:
	SandboxLayer3D();

	void onAttach() override;
	void onDetach() override;
	void onUpdate(std::chrono::milliseconds timestep) override;
	void onEvent(Nest::Event &e) override;

private:
	Nest::PerspectiveCamController3D m_camController;

	Nest::Ref<Nest::Shader> m_shader3D, m_teapotShader;
	Nest::StaticMesh m_teapot, m_suzanne;
};