#include "SandboxLayer.h"

#include "MovingAverage.h"

SandboxLayer::SandboxLayer()
	: m_camera(chcl::AlignedRect(0, 0, 1280, 720))
{
	auto tex1 = Nest::PNGLoader("res\\PNG\\PngSuite.png")().toTexture();
	auto tex2 = Nest::PNGLoader("res\\Test_card.png")().toTexture();
	auto tex3 = Nest::PNGLoader("res\\PNG\\oi4n2c16.png")().toTexture();
	auto tex4 = Nest::PNGLoader("res\\PNG\\oi9n2c16.png")().toTexture();
	auto tex5 = Nest::PNGLoader("res\\PNG\\oi1n0g16.png")().toTexture();

	Nest::ECS::Entity sprite1, sprite2, sprite3, sprite4, sprite5;
	Nest::ECS::SpriteComponent &spriteSys = Nest::ECS::SpriteComponent::Get();
	spriteSys.attachToEntity(sprite1, tex1, chcl::Vector2<float>(180, 200), 1.0f, 0.f);
	spriteSys.attachToEntity(sprite2, tex2, chcl::Vector2<float>(180, 500), 0.4f, 0.f);
	spriteSys.attachToEntity(sprite3, tex3, chcl::Vector2<float>(400, 200), 4.f, 0.f);
	spriteSys.attachToEntity(sprite4, tex4, chcl::Vector2<float>(400, 350), 4.f, 0.f);
	spriteSys.attachToEntity(sprite5, tex5, chcl::Vector2<float>(400, 500), 4.f, 0.f);

	m_scene.addEntity(std::move(sprite1));
	m_scene.addEntity(std::move(sprite2));
	m_scene.addEntity(std::move(sprite3));
	m_scene.addEntity(std::move(sprite4));
	m_scene.addEntity(std::move(sprite5));

	m_scene.addSystem(Nest::ECS::SpriteSystem::Create());

	//m_testFont = Nest::TTFLoader("res\\Fonts\\OpenSans\\OpenSans-Regular.ttf").get();
	//m_testFont = Nest::TTFLoader("res\\Fonts\\Arial\\ARIAL.TTF").get();
	m_testFont = Nest::TTFLoader("res\\Fonts\\Wingdings\\wingding.ttf").get();

	std::string loremIpsum = R"(Lorem ipsum dolor sit amet,
consectetur adipiscing elit,
sed do eiusmod tempor incididunt
ut labore et dolore magna aliqua.)";

	Nest::ECS::Entity box1, box2;
	Nest::ECS::TextBoxComp &textBoxComp = Nest::ECS::TextBoxComp::Get();
	textBoxComp.attachToEntity(box1, chcl::Vector2<float>(600, 450), "(*^*)", 100.f, m_testFont);
	textBoxComp.attachToEntity(box2, chcl::Vector2<float>(600, 350), loremIpsum, 30.f, m_testFont);

	//NE_TRACE("{}", (-float(-164) - std::sqrtf(-164 * -164 - 4 * -89 * 253)) / (2.0 * float(-89)) == 1.0f);

	m_scene.addEntity(std::move(box1));
	m_scene.addEntity(std::move(box2));

	m_scene.addSystem(std::move(Nest::ECS::TextBoxSystem::Create()));
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

	m_scene.update();

	static MovingAverage fpsAverage{ 200 };

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

	//Nest::Renderer2D::drawCircle({ 1280 / 2, 720 / 2 }, { 1280 - 2, 720 - 2 }, 1, 0.f);
	//Nest::Renderer2D::drawLineQuad({ 200, 500 }, { 150, 40 }, chcl::toRadians(30.f));
	//Nest::Renderer2D::drawQuad({ 200, 200 }, { 200, 150 });
	//Nest::Renderer2D::drawTexturedQuad({ 960, 360 }, { 640, 480 }, m_tex2);
	//Nest::Renderer2D::drawTexturedQuad({ 640, 360 }, { 160, 160 }, m_tex3);
	//Nest::Renderer2D::drawTexturedQuad({ 800, 360 }, { 160, 160 }, m_tex4);
	//Nest::Renderer2D::drawTexturedQuad({ 960, 360 }, { 160, 160 }, m_tex5);
	//Nest::Renderer2D::drawTexturedQuad({ 640, 360 }, { 640, 640 }, m_tex1);
}

void SandboxLayer::onEvent(Nest::Event & e)
{
}
