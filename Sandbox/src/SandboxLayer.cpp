#include "SandboxLayer.h"

#include "MovingAverage.h"

SandboxLayer::SandboxLayer()
	: m_camera(chcl::AlignedRect(0, 0, 1280, 720))
{
	m_tex1 = Nest::PNGLoader("res\\PNG\\PngSuite.png")().toTexture();
	m_tex2 = Nest::PNGLoader("res\\Test_card.png")().toTexture();
	//m_tex3 = Nest::PNGLoader("res\\PNG\\oi4n2c16.png")().toTexture();
	//m_tex4 = Nest::PNGLoader("res\\PNG\\oi9n2c16.png")().toTexture();
	//m_tex5 = Nest::PNGLoader("res\\PNG\\oi1n0g16.png")().toTexture();

	//m_testFont = Nest::TTFLoader("res\\Fonts\\Arial\\ARIAL.TTF").get();
	m_testFont = Nest::TTFLoader("res\\Fonts\\OpenSans\\OpenSans-Regular.ttf").get();

	//NE_TRACE("{}", (-float(-164) - std::sqrtf(-164 * -164 - 4 * -89 * 253)) / (2.0 * float(-89)) == 1.0f);
}

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

	static float testSize = 5.f;

	Nest::Renderer2D::drawText({ 50, 100 }, testSize, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", m_testFont);
	Nest::Renderer2D::drawText({ 50, 250 }, testSize, "abcdefghijklmnopqrstuvwxyz", m_testFont);
	Nest::Renderer2D::drawText({ 50, 400 }, testSize, "1234567890!@#$%^&*()[]{}<>", m_testFont);
	Nest::Renderer2D::drawText({ 50, 550 }, testSize, "(*^*)", m_testFont);

	testSize += timestep.count() / 100.f;
	if (testSize > 200.f)
		testSize = 5.f;

	static MovingAverage fpsAverage{ 5 };

	if (timestep.count() > 0)
		fpsAverage.add(1000.f / float(timestep.count()));

	if (fpsAverage.getSize() > 1 && Nest::Input::WasKeyPressed(NE_KEY_COMMA))
	{
		fpsAverage.setSize(fpsAverage.getSize() - 1);
		NE_TRACE("Set fps window to {}", fpsAverage.getSize());
	}
	
	if (Nest::Input::WasKeyPressed(NE_KEY_PERIOD))
	{
		fpsAverage.setSize(fpsAverage.getSize() + 1);
		NE_TRACE("Set fps window to {}", fpsAverage.getSize());
	}

	static Nest::Formatter fpsFormat;
	Nest::Renderer2D::drawText({ 1100, 700 }, 20, fpsFormat.format("FPS: {:.0}", fpsAverage.get()), m_testFont);

	static float t = 0.f;

	//Nest::Renderer2D::drawText({ 100, 150 }, 500.f + 2.f * std::sin(t), "8", m_testFont);

	t += timestep.count() / 3000.f;

	//Nest::Renderer2D::drawCircle({ 1280 / 2, 720 / 2 }, { 1280 - 2, 720 - 2 }, 1, 0.f);
	//Nest::Renderer2D::drawLineQuad({ 200, 500 }, { 150, 40 }, chcl::toRadians(30.f));
	//Nest::Renderer2D::drawQuad({ 200, 200 }, { 200, 150 });
	//Nest::Renderer2D::drawTexturedQuad({ 640 - 256, 360 }, { 512, 512 }, m_tex1);
	//Nest::Renderer2D::drawTexturedQuad({ 960, 360 }, { 640, 480 }, m_tex2);
	//Nest::Renderer2D::drawTexturedQuad({ 640, 360 }, { 160, 160 }, m_tex3);
	//Nest::Renderer2D::drawTexturedQuad({ 800, 360 }, { 160, 160 }, m_tex4);
	//Nest::Renderer2D::drawTexturedQuad({ 960, 360 }, { 160, 160 }, m_tex5);
	//Nest::Renderer2D::drawTexturedQuad({ 640, 360 }, { 640, 640 }, m_tex1);
}

void SandboxLayer::onEvent(Nest::Event & e)
{
}
