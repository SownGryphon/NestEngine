#pragma once

#include <Nest.h>

class ProfileLayer : public Nest::Layer
{
public:
	ProfileLayer();

	void onUpdate(std::chrono::milliseconds timestep) override;

private:
	Nest::OrthographicCamera2D m_camera;
};