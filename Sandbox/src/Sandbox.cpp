#include <Nest.h>

#include <random>

class Boid
{
	static chcl::Vector2<float> modelVertices[3];

public:
	chcl::Vector2<float> m_position;
	float m_rotation;

	Boid() : m_position(0.f), m_rotation(0) {};

	Boid(float x, float y, float angle)
		: m_position(x, y), m_rotation(angle)
	{
	}
};

chcl::Vector2<float> Boid::modelVertices[3] = {
		{5, 0},
		{-3, 3},
		{-3, -3}
};

class BoidDrawer
{
	const static unsigned int boidCount = 500;
	Boid boids[boidCount];
	Nest::Ref<Nest::VertexArray> va;
	Nest::Ref<Nest::Shader> shader;

public:
	BoidDrawer()
	{
		std::srand(0);
		for (unsigned int i = 0; i < boidCount; ++i)
		{
			boids[i] = Boid(
				std::rand() % 1280,
				std::rand() % 720,
				std::rand()
			);
		}

		va = Nest::createRef<Nest::VertexArray>();
		Nest::Ref<Nest::VertexBuffer> vb = Nest::createRef<Nest::VertexBuffer>(boidCount * 3 * sizeof(float), boids);
		Nest::Ref<Nest::VertexBufferLayout> vbl = Nest::createRef<Nest::VertexBufferLayout>();
		vbl->push<float>(2);
		vbl->push<float>(1);
		va->bindBuffers(vb, vbl);

		shader = Nest::createRef<Nest::Shader>("res/shaders/Boid.glsl");
	}

	void draw()
	{
		shader->bind();
		shader->setUniform1f("formatTest", 1.f);
		Nest::Renderer::drawPoints(*shader, *va, boidCount);
	}
};

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
		Nest::OrthograhicCamera camera(0, 1280, 0, 720, -1, 1);

		Nest::Renderer::init();
		Nest::Renderer::beginScene(camera);

		BoidDrawer boids;

		while (true)
		{
			Nest::Renderer::clear();

			//Nest::Renderer2D::drawQuad({ 80, 45 }, { 40, 15 }, chcl::toRadians(30.f), { 0.3, 0.3, 0.9, 1.0 });
			//Nest::Renderer2D::drawQuad({ 30, 60 }, { 20, 35 }, chcl::toRadians(10.f), { 0.8, 0.3, 0.1, 1.0 });
			boids.draw();

			m_window->onUpdate();
		}
	}
};

Nest::Application* Nest::createApplication()
{
	return new Sandbox();
}