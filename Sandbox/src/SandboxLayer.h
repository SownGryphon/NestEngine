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
	Nest::Ref<Nest::Texture> m_tex1, m_tex2, m_tex3, m_tex4, m_tex5;
	Nest::Ref<Nest::Font> m_testFont;
};