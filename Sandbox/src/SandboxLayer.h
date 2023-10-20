#pragma once

#include <Nest.h>

class SandboxLayer : public Nest::Layer
{
public:
	SandboxLayer();

	void onAttach() override;
	void onDetach() override;
	void onUpdate() override;
	void onEvent(Nest::Event &e) override;

private:
	Nest::OrthograhicCamera m_camera;
};