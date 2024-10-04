#include "ProfileLayer.h"

#include <thread>

ProfileLayer::ProfileLayer() :
	m_camera({ 0.f, 0.f }, { 1280.f, 720.f })
{}

void ProfileLayer::onUpdate(std::chrono::milliseconds timestep)
{
	Nest::Renderer2D::beginScene(m_camera.getVPM());

	Nest::Renderer2D::drawQuad({ 0.f, 0.f }, { 400.f, 400.f });

	//std::this_thread::sleep_for(std::chrono::milliseconds(20));
}