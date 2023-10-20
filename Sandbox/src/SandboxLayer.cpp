#include "SandboxLayer.h"

#include <Nest.h>

SandboxLayer::SandboxLayer()
	: m_camera(0, 1280, 0, 720, -1, 1)
{
}

void SandboxLayer::onAttach()
{
}

void SandboxLayer::onDetach()
{
}

void SandboxLayer::onUpdate()
{
	Nest::RenderCommand::clear();

	Nest::Renderer2D::beginScene(m_camera);
	Nest::Renderer2D::drawLineQuad({ 100, 100 }, { 150, 40 }, chcl::toRadians(30.f));
	Nest::Renderer2D::drawQuad({ 100, 100 }, { 80, 50 });
}

void SandboxLayer::onEvent(Nest::Event & e)
{
}
