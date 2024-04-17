#include "EvolutionSimLayer.h"

float EvolutionSimLayer::Food::s_foodEnergy = 40.f;

EvolutionSimLayer::EvolutionSimLayer(float simAreaWidth, float simAreaHeight, float aspectRatio)
	: m_simArea(0, 0, simAreaWidth, simAreaHeight),
	m_camera(aspectRatio),
	m_agentQT(m_simArea),
	m_foodQT(m_simArea)
{
	m_camera.fit(m_simArea);

	Agent::SetBounds(m_simArea);
	Agent::EnergyCosts costs = { 0.f };
	costs.moveCost = 0.0008f;
	costs.accelerateCost = 0.f;
	costs.turnCost = 0.f;
	costs.viewCost = 0.f;
	costs.liveCost = 0.001f;
	Agent::SetEnergyCosts(costs);

	Nest::Random::init(3);

	initPopulation(400, 100);
}

void EvolutionSimLayer::onAttach()
{
	m_agentShader = Nest::createRef<Nest::Shader>("res/shaders/Boid.glsl");

	Nest::Ref<Nest::VertexBuffer> vb = Nest::createRef<Nest::VertexBuffer>(3 * sizeof(float));
	Nest::Ref<Nest::VertexBufferLayout> vbl = Nest::createRef<Nest::VertexBufferLayout>();
	vbl->push<float>(2);
	vbl->push<float>(1);
	m_agentVertices = Nest::createRef<Nest::VertexArray>();
	m_agentVertices->bindBuffers(vb, vbl);
}

void EvolutionSimLayer::onDetach()
{
}

void EvolutionSimLayer::onUpdate(std::chrono::milliseconds timestep)
{
	m_camera.onUpdate();

	m_agentQT.clear();
	m_foodQT.clear();

	if (Nest::Random::randomInt(0, 10) == 0)
	{
		m_foods.emplace_back(chcl::Vector2<float>(
			Nest::Random::random(0, m_simArea.size.x),
			Nest::Random::random(0, m_simArea.size.y)
		));
	}

	for (Agent &agent : m_agents)
		m_agentQT.addElement(&agent, agent.position);

	for (Food &food : m_foods)
		m_foodQT.addElement(&food, food.position);

	// Do distance related calculations for all agents
	for (Agent &agent : m_agents)
	{
		if (!agent.isAdult())
			continue;

		Agent* closestAgent = &agent;
		chcl::Vector2<float> closestFood = agent.position;
		float closestAgentDist = agent.genome.viewRadius;
		float closestFoodDist = agent.genome.viewRadius;

		for (auto seenAgents = m_agentQT.getElements(chcl::Circle(agent.position, agent.genome.viewRadius));
			Agent *other : seenAgents)
		{
			if (other == &agent)
				continue;

			float dist = (agent.position - other->position).mag();
			if (other->isAdult() && other->alive
				&& agent.aggressive
				&& agent.genome.size > other->genome.size
				&& dist < agent.genome.size + other->genome.size)
			{
				other->alive = false;
				agent.energy += other->energy;
				continue;
			}

			if (dist < closestAgentDist)
			{
				closestAgent = other;
				closestAgentDist = dist;
			}
		}

		for (auto seenFood = m_foodQT.getElements(chcl::Circle(agent.position, agent.genome.viewRadius));
			auto &food : seenFood)
		{
			float dist = (agent.position - food->position).mag();
			if (dist < agent.genome.size + Food::s_foodEnergy / 2)
			{
				food->eaten = true;
				agent.energy += food->s_foodEnergy;
				continue;
			}

			if (agent.isAdult() && dist < closestFoodDist)
			{
				closestFood = food->position;
				closestFoodDist = dist;
			}
		}

		agent.updateNetwork(closestFood, *closestAgent);
	}

	std::erase_if(m_agents, [](const Agent &agent) { return !agent.alive; });
	std::erase_if(m_foods, [](const Food &food) { return food.eaten; });

	Nest::Renderer::beginScene(m_camera.getCamera());
	Nest::Renderer2D::beginScene(m_camera.getCamera());

	Nest::RenderCommand::clear();

	for (Food food : m_foods)
	{
		Nest::Renderer2D::drawCircle(food.position, food.s_foodEnergy / 2, { 0.1f, 0.9f, 0.2f, 1.f });
	}

	std::vector<Agent> newAgents;
	for (Agent &agent : m_agents)
	{
		agent.update();

		if (agent.energy > agent.genome.reproductionThreshold)
			newAgents.push_back(agent.createChild());

		drawAgent(agent);
	}
	m_agents.insert(m_agents.end(), newAgents.begin(), newAgents.end());
}

void EvolutionSimLayer::onEvent(Nest::Event & e)
{
	Nest::EventDispatcher dispatcher(e);
	dispatcher.dispatchEvent<Nest::KeyPressedEvent>(NE_BIND_EVENT_FN(EvolutionSimLayer::onKeyEvent));
	m_camera.onEvent(e);
}

bool EvolutionSimLayer::onKeyEvent(Nest::KeyPressedEvent &e)
{
	switch (e.getKeyCode())
	{
		case NE_KEY_R:
		{
			m_camera.fit(m_simArea);
			return true;
		}
		case NE_KEY_V:
		{
			m_showBoidView = !m_showBoidView;
			return true;
		}
		case NE_KEY_Q:
		{
			m_showQuadTree = !m_showQuadTree;
			return true;
		}
	}

	return false;
}

void EvolutionSimLayer::initPopulation(unsigned int agentCount, unsigned int foodCount)
{
	m_agents.reserve(agentCount);
	for (unsigned int i = 0; i < agentCount; ++i)
	{
		m_agents.push_back(Agent(
			Nest::Random::random(0, m_simArea.size.x),
			Nest::Random::random(0, m_simArea.size.y),
			Nest::Random::random(-chcl::PI, chcl::PI),
			500.f
		));
		m_agents.back().age = m_agents.back().genome.growTime;
	}

	m_foods.reserve(foodCount);
	for (unsigned int i = 0; i < foodCount; ++i)
	{
		m_foods.emplace_back(chcl::Vector2<float>(
			Nest::Random::random(0, m_simArea.size.x),
			Nest::Random::random(0, m_simArea.size.y)
		));
	}
}

void EvolutionSimLayer::drawAgent(const Agent &agent)
{
	if (!agent.isAdult())
	{
		Nest::Renderer2D::drawCircle(agent.position, agent.genome.size / 2);
		return;
	}

	m_agentShader->bind();
	m_agentShader->setUniform1f("u_boidSize", agent.genome.size);

	m_agentVertices->bind();
	m_agentVertices->getVertexBuffer()->setData(sizeof(chcl::Vector2<float>), (void*)&agent.position, 0);
	m_agentVertices->getVertexBuffer()->setData(sizeof(float), (void*)&agent.direction, sizeof(chcl::Vector2<float>));

	Nest::Renderer::drawPoints(*m_agentShader, *m_agentVertices, 1);
}
