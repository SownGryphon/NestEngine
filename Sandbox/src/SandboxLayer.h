#pragma once

#include <Nest.h>

#include <geometry/Vector3.h>

class SandboxLayer : public Nest::Layer
{
public:
	SandboxLayer();

	void onAttach() override;
	void onDetach() override;
	void onUpdate(std::chrono::milliseconds timestep) override;
	void onEvent(Nest::Event &e) override;

private:
	Nest::OrthographicCamera2D m_camera;
	Nest::Ref<Nest::Font> m_testFont;

	Nest::ECS::Scene m_scene;
};