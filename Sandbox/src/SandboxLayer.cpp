#include "SandboxLayer.h"

SandboxLayer::SandboxLayer()
	: m_camera(chcl::AlignedRect(0, 0, 1280, 720))
{}

void SandboxLayer::onAttach()
{
}

void SandboxLayer::onDetach()
{
}

void SandboxLayer::onUpdate(std::chrono::milliseconds timestep)
{
	Nest::RenderCommand::clear();

	Nest::Renderer2D::beginScene(m_camera.getVPM());

	Nest::Renderer2D::drawCircle({ 1280/2, 720/2 }, { 1280-2, 720-2 }, 1, 0.f);
	Nest::Renderer2D::drawLineQuad({ 200, 500 }, { 150, 40 }, chcl::toRadians(30.f));
	Nest::Renderer2D::drawQuad({ 100, 100 }, { 80, 50 });
}

void SandboxLayer::onEvent(Nest::Event & e)
{
}
