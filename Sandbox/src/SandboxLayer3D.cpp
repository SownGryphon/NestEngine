#include "SandboxLayer3D.h"

#include <Nest.h>

SandboxLayer3D::SandboxLayer3D() :
	m_camController(chcl::toRadians(45.f), 1280.f / 720.f, 0.1f, 100.f, { 0.f, 2.f, 6.f }, chcl::toRadians(0.f), chcl::toRadians(0.f), 5.f, 0.1f)
{
	m_shader3D = Nest::Shader::FromFile("res/BasicShader3D.glsl");
	m_teapotShader = Nest::Shader::FromFile(std::vector<std::string>{ "res/Teapot.glsl.vert", "res/BasicShaded.glsl.frag" });
	m_teapot = Nest::StaticMesh::LoadOBJ("res/OBJ/newell_teaset/teapot.obj", false, true);
	m_suzanne = Nest::StaticMesh::LoadOBJ("res/OBJ/suzanne.obj", false, true);
}

void SandboxLayer3D::onAttach()
{
}

void SandboxLayer3D::onDetach()
{
}

void SandboxLayer3D::onUpdate(std::chrono::milliseconds timestep)
{
	m_camController.onUpdate(timestep);

	Nest::RenderCommand::clear();

	Nest::Renderer3D::beginScene(m_camController.getVPM());

	Nest::Renderer3D::drawAxes({ 0.f, 0.f, 0.f }, 2.f);
	Nest::Renderer3D::drawCuboid({ 0.f, 0.f, -4.f }, 4.f, 1.f, m_shader3D);
	Nest::Renderer3D::drawCuboid({ 4.f, 0.f, 3.f }, 3.f, { 0.9f, 0.5f, 0.1f, 1.f }, m_shader3D);
	Nest::Renderer3D::drawCuboid({ -1.f, 5.f, 0.f }, 2.5f, chcl::toRadians(-20.f), chcl::toRadians(30.f), 1.f, m_shader3D);
	Nest::Renderer3D::drawCuboid({ 1.f, -3.f, 1.f }, 2.f, { 0.f, 0.5f, 0.2f, 1.f });

	Nest::Renderer3D::drawMesh({ -6.f, -1.f, 2.f }, 0.5f, m_teapot, m_teapotShader);
	Nest::Renderer3D::drawMesh({ -8.5f, -1.f, 2.f }, 0.5f, m_teapot, m_shader3D);
	Nest::Renderer3D::drawMesh({ -6.f, 0.f, -2.f }, 1.f, m_suzanne, m_shader3D);
	Nest::Renderer3D::drawMesh({ -8.5f, 0.f, -2.f }, 1.f, m_suzanne, m_teapotShader);
}

void SandboxLayer3D::onEvent(Nest::Event &e)
{
}