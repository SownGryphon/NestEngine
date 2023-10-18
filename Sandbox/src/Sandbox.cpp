#include <Nest.h>

class Sandbox : public Nest::Application
{
public:
	Sandbox()
		: Application()
	{

	}

	~Sandbox()
	{

	}

	void run()
	{
		Nest::OrthograhicCamera camera(0, 160, 0, 90, -1, 1);

		Nest::Renderer::init();
		Nest::Renderer::beginScene(camera);

		while (true)
		{
			Nest::Renderer::clear();

			Nest::Renderer2D::drawQuad({ 80, 45 }, { 40, 15 }, chcl::toRadians(30.f));
			Nest::Renderer2D::drawQuad({ 30, 60 }, { 20, 35 }, chcl::toRadians(10.f), { 0.8, 0.3, 0.1, 1.0 });

			m_window->onUpdate();
		}
	}
};

Nest::Application* Nest::createApplication()
{
	return new Sandbox();
}