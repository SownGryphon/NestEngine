#include "SandboxLayer.h"

#include <Nest.h>

SandboxLayer::SandboxLayer()
	: m_camera(0, 1280, 0, 720, -1, 1),
	m_camera3D({ 0, 0, 2 }, 0, 0)
{
	m_shader = Nest::Shader::FromFile("res/shaders/Test.vert", "res/shaders/Raymarch.frag");
	Nest::Random::init(0);

	m_shader->bind();
	auto &win = Nest::Application::GetInstance().getWindow();
	m_shader->setUniform1f("u_aspect", float(win.getWidth()) / win.getHeight());
}

void SandboxLayer::onAttach()
{
}

void SandboxLayer::onDetach()
{
}

void SandboxLayer::onUpdate(std::chrono::milliseconds timestep)
{
	//m_camera3D.update(timestep);

	Nest::RenderCommand::clear();

	Nest::Renderer2D::beginScene(m_camera);

	Nest::Renderer2D::drawCircle({ 700, 400 }, { 120, 230 }, 1, { 0.2f, 0.2f, 0.2f, 1.f });
	Nest::Renderer2D::drawLineQuad({ 200, 500 }, { 150, 40 }, chcl::toRadians(30.f));
	Nest::Renderer2D::drawQuad({ 100, 100 }, { 80, 50 });

	//m_shader->bind();
	//m_shader->setUniform1f("u_time", float(Nest::Time::getTimeMillis().count()));
	//m_shader->setUniform3f("u_cameraPos", m_camera3D.getPosition());
	//m_shader->setUniformMat3("u_cameraRot", m_camera3D.getRotationTransform());
	//Nest::Renderer2D::drawFullScreenQuad(m_shader);
}

void SandboxLayer::onEvent(Nest::Event & e)
{
}
