#include "NEATBoidLayer.h"

#include <random>

#include <geometry/Circle.h>
#include <geometry/Vector2.h>
#include <geometry/Vector3.h>
#include <maths/Continuous.h>

NEATBoidLayer::BoidData NEATBoidLayer::s_preyData;
NEATBoidLayer::BoidData NEATBoidLayer::s_predatorData;

NEATBoidLayer::NEATBoidLayer(chcl::Rect simArea)
	: m_boidQT(simArea),
	m_simArea(simArea),
	m_simSpaceConstraint(simArea.bl(), simArea.tr()),
	m_camera(simArea)
{
	s_preyData.viewDistance = 80.f;
	s_preyData.maxSpeed = 1.5f;
	s_preyData.turnSpeed = chcl::toRadians(3.f);
	s_preyData.acceleration = 0.09f;
	s_preyData.size = 5.f;
	s_preyData.growTime = 120;
	s_preyData.reproductionThreshold = 6.7f;
	s_preyData.mutationProbs = {
		0.15f,
		0.03f,
		0.8f,
		0.3f,
		0.1f
	};

	s_predatorData.viewDistance = 150.f;
	s_predatorData.maxSpeed = 2.7f;
	s_predatorData.turnSpeed = chcl::toRadians(1.4f);
	s_predatorData.acceleration = 0.05f;
	s_predatorData.size = 14.f;
	s_predatorData.growTime = 300;
	s_predatorData.reproductionThreshold = 1.2f;
	s_predatorData.mutationProbs = {
		0.15f,
		0.03f,
		0.8f,
		0.3f,
		0.1f
	};

	std::srand(14);
	createPopulation(400, 70);
	m_boidTarget.init(m_simSpaceConstraint.clamp({ float(std::rand()), float(std::rand()) }));
}

NEATBoidLayer::Boid::Boid(chcl::Vector2<float> position, float angle, bool isPredator)
	: position(position), angle(angle), isPredator(isPredator),
	network(10, 4, 13)
{
	age = getStaticData().growTime;
}

NEATBoidLayer::Boid NEATBoidLayer::Boid::createOffspring() const
{
	Boid child(position, angle, isPredator);
	child.age = 0;
	child.network = network;
	child.network.mutate(NEATBoidLayer::GetBoidStaticData(isPredator).mutationProbs);
	return child;
}

void NEATBoidLayer::onAttach()
{
	m_boidShader = Nest::createRef<Nest::Shader>("res/shaders/Boid.glsl");

	Nest::Ref<Nest::VertexBuffer> vb = Nest::createRef<Nest::VertexBuffer>(3 * sizeof(float));
	Nest::Ref<Nest::VertexBufferLayout> vbl = Nest::createRef<Nest::VertexBufferLayout>();
	vbl->push<float>(2);
	vbl->push<float>(1);
	m_boidVertices = Nest::createRef<Nest::VertexArray>();
	m_boidVertices->bindBuffers(vb, vbl);
}

void NEATBoidLayer::onDetach()
{
}

void NEATBoidLayer::onUpdate(std::chrono::milliseconds timestep)
{
	m_camera.onUpdate();

	m_boidQT.clear();

	// Initialize QuadTree and search distances
	for (unsigned int i = 0; i < m_boids.size(); ++i)
	{
		m_boidQT.addElement(i, m_boids[i].position);
	}

	for (unsigned int i = 0; i < m_boids.size(); ++i)
	{
		// Get all boids within view distance
		auto nearBoids = m_boidQT.getElements(chcl::Circle(m_boids[i].position, m_boids[i].getStaticData().viewDistance));
		float nearestPreyDist = m_boids[i].getStaticData().viewDistance,
			nearestPredatorDist = m_boids[i].getStaticData().viewDistance;
		unsigned int kills = 0;

		auto &boid = m_boids[i];
		boid.closestPrey = i;
		boid.closestPredator = i;

		for (unsigned int j : nearBoids)
		{
			// Skip itself
			if (i == j)
				continue;

			auto &otherBoid = m_boids[j];

			float dist = (boid.position - otherBoid.position).mag();

			if (!otherBoid.isPredator && dist < nearestPreyDist)
			{
				nearestPreyDist = dist;
				boid.closestPrey = j;
			}
			else if (otherBoid.isPredator && dist < nearestPredatorDist)
			{
				nearestPredatorDist = dist;
				boid.closestPredator = j;
			}

			if (dist < boid.getStaticData().size + otherBoid.getStaticData().size
				&& (boid.isPredator || !otherBoid.isPredator)
				&& isAdult(i) && isAdult(j))
			{
				otherBoid.isAlive = false;
				++kills;
			}
		}

		if (!boid.isAlive)
			continue;

		if (boid.isPredator)
			updateReproductionPredator(i, kills);
		else if (isAdult(i))
			updateReproductionPassive(i);

		updateBoidNetwork(i);
	}

	drawBoids();
	updateBoids();

	m_boidTarget.wander();
	m_boidTarget.position = m_simSpaceConstraint.clamp(m_boidTarget.position);
}

void NEATBoidLayer::onEvent(Nest::Event &e)
{
	Nest::EventDispatcher dispatcher(e);
	dispatcher.dispatchEvent<Nest::KeyPressedEvent>(NE_BIND_EVENT_FN(NEATBoidLayer::keyPressHandler));
	m_camera.onEvent(e);
}

void NEATBoidLayer::updateBoidNetwork(unsigned int boidIndex)
{
	auto &boid = m_boids[boidIndex];
	chcl::Vector2 forward = chcl::Vector2<>::FromAngle(boid.angle),
		sideways = chcl::Vector2<>::FromAngle(boid.angle - chcl::PI / 2),
		targetOffset = m_simSpaceConstraint.diffMin(boid.position, m_boidTarget.position);
	// Network inputs:
	// - Target forward offset
	// - Target sideways offset
	// - Closest prey forward offset - [-1, 1] (1 if none seen)
	// - Closest prey sideways offset - [-1, 1] (0 if none seen)
	// - Closest prey angle difference - [-PI, PI] (0 if none seen)
	// - Closest prey speed - [0, 1] (own speed if none seen)
	// - Closest predator forward offset - [-1, 1] (-1 if none seen)
	// - Closest predator sideways offset - [-1, 1] (0 if none seen)
	// - Closest predator angle difference - [-PI, PI] (0 if none seen)
	// - Closest predator speed - [0, 1] (own speed if none seen)
	// - Own speed - [0, 1]
	std::vector<float> inputs = {
		chcl::Vector2<>::Dot(forward, targetOffset) / boid.getStaticData().viewDistance,	// 0
		chcl::Vector2<>::Dot(sideways, targetOffset) / boid.getStaticData().viewDistance,	// 1
		1.f,											// 2
		0.f,											// 3
		0.f,											// 4
		boid.speed / boid.getStaticData().maxSpeed,		// 5
		1.f,											// 6
		0.f,											// 7
		0.f,											// 8
		boid.speed / boid.getStaticData().maxSpeed,		// 9
		boid.speed / boid.getStaticData().maxSpeed		// 10
	};

	if (boidIndex != boid.closestPrey)
	{
		auto &prey = m_boids[boid.closestPrey];
		chcl::Vector2 boidOffset = prey.position - boid.position;
		inputs[2] = chcl::Vector2<>::Dot(forward, boidOffset) / boid.getStaticData().viewDistance;
		inputs[3] = chcl::Vector2<>::Dot(sideways, boidOffset) / boid.getStaticData().viewDistance;
		inputs[4] = chcl::radianSolver.diffMin(boid.angle, prey.angle);
		inputs[5] = prey.speed / prey.getStaticData().maxSpeed;
	}

	if (boidIndex != boid.closestPredator)
	{
		auto &predator = m_boids[boid.closestPredator];
		chcl::Vector2 boidOffset = predator.position - boid.position;
		inputs[6] = chcl::Vector2<>::Dot(forward, boidOffset) / boid.getStaticData().viewDistance;
		inputs[7] = chcl::Vector2<>::Dot(sideways, boidOffset) / boid.getStaticData().viewDistance;
		inputs[8] = chcl::radianSolver.diffMin(boid.angle, predator.angle);
		inputs[9] = predator.speed / predator.getStaticData().maxSpeed;
	}

	boid.network.setInputs(inputs);
	boid.network.process();
}

void NEATBoidLayer::updateReproductionPassive(unsigned int boid)
{
	m_boids[boid].reproductionTime += (m_boids[boid].speed / s_preyData.maxSpeed) / (1 + m_simSpaceConstraint.diffMin(m_boids[boid].position, m_boidTarget.position).mag());
}

void NEATBoidLayer::updateReproductionPredator(unsigned int boid, unsigned int kills)
{
	m_boids[boid].reproductionTime += kills * (s_preyData.size / s_predatorData.size);
}

void NEATBoidLayer::updateBoids()
{
	for (unsigned int i = 0; i < m_boids.size(); ++i)
	{
		auto &boid = m_boids[i];

		if (!boid.isAlive)
		{
			removeBoid(i);
			--i;
			continue;
		}

		++boid.age;

		BoidData& boidData = boid.getStaticData();

		auto netOutputs = boid.network.getOutputs();
		boid.speed += (netOutputs[0] - netOutputs[1]) * boidData.acceleration;
		boid.angle = chcl::radianSolver.clamp(boid.angle + (netOutputs[2] - netOutputs[3]) * boidData.turnSpeed);

		if (boid.speed < 0)
			boid.speed = 0;
		else if (boid.speed > boidData.maxSpeed)
			boid.speed = boidData.maxSpeed;

		chcl::Vector2<float> dir = chcl::Vector2<>::FromAngle(boid.angle);
		boid.position += dir * boid.speed;

		boid.position = m_simSpaceConstraint.clamp(boid.position);

		if (boid.reproductionTime > boidData.reproductionThreshold)
		{
			boid.reproductionTime -= boidData.reproductionThreshold;
			addBoid(i);
		}
	}
}

void NEATBoidLayer::drawBoids()
{
	Nest::RenderCommand::clear();

	Nest::Renderer2D::beginScene(m_camera.getCamera());
	Nest::Renderer::beginScene(m_camera.getCamera());

	if (m_showQuadTree)
	{
		drawQTRegion(m_boidQT.getRegion());
	}

	if (m_showBoidView)
	{
		for (unsigned int i = 0; i < m_boids.size(); ++i)
		{
			chcl::Vector4 color = isAdult(i) ? chcl::Vector4<float>(0.7f, 0.7f, 0.7f, 0.4f) : chcl::Vector4<float>(0.85f, 0.85f, 0.85f, 0.15f);

			Nest::Renderer2D::drawCircle(m_boids[i].position, m_boids[i].getStaticData().viewDistance * 2, color);
		}
		for (unsigned int i = 0; i < m_boids.size(); ++i)
		{
			if (m_boids[i].closestPrey != i)
				Nest::Renderer2D::drawLine(m_boids[i].position, m_boids[m_boids[i].closestPrey].position);
			if (m_boids[i].closestPredator != i)
				Nest::Renderer2D::drawLine(m_boids[i].position, m_boids[m_boids[i].closestPredator].position);
		}
	}

	Nest::Renderer2D::drawCircle(m_boidTarget.position, 23.f, { 0.1f, 0.9f, 0.1f, 1.f });

	auto vertexBuffer = m_boidVertices->getVertexBuffer();
	for (unsigned int i = 0; i < m_boids.size(); ++i)
	{
		vertexBuffer->setData(sizeof(chcl::Vector2<float>), &m_boids[i].position, 0);
		vertexBuffer->setData(sizeof(float), &m_boids[i].angle, 2 * sizeof(float));

		// To be accessed by:
		// - Age
		// - Predator status
		static chcl::Vector4<float> colors[2][2] = {
			{
				{ 0.5f, 0.9f, 0.9f, 1.f },
				{ 0.9f, 0.5f, 0.2f, 0.9f }
			},
			{
				{ 1.f, 1.f, 1.f, 1.f },
				{ 1.f, 0.2f, 0.1f, 1.0f }
			}
		};

		m_boidShader->bind();
		m_boidShader->setUniform4f("u_color", colors[isAdult(i)][m_boids[i].isPredator]);
		m_boidShader->setUniform1f("u_boidSize", m_boids[i].getStaticData().size);

		Nest::Renderer::drawPoints(*m_boidShader, *m_boidVertices, 1);
	}
}

void NEATBoidLayer::drawQTRegion(const QTType::QTRegion &region)
{
	Nest::Renderer2D::drawLineQuad(region.area.center(), region.area.size);

	if (region.subdivided)
	{
		for (const auto &child : region.children)
			drawQTRegion(child);
	}
}

bool NEATBoidLayer::keyPressHandler(Nest::KeyPressedEvent &e)
{
	if (e.getKeyCode() == NE_KEY_Q)
	{
		m_showQuadTree = !m_showQuadTree;
		return true;
	}

	if (e.getKeyCode() == NE_KEY_T)
	{
		m_showBoidView = !m_showBoidView;
		return true;
	}

	if (e.getKeyCode() == NE_KEY_R)
	{
		m_camera.setPosition(m_simArea.center());
		m_camera.setZoomFactor(2 / m_simArea.size.y);
		return true;
	}

	return false;
}

bool NEATBoidLayer::isAdult(unsigned int boid) const
{
	return m_boids[boid].age > m_boids[boid].getStaticData().growTime;
}

NEATBoidLayer::BoidData& NEATBoidLayer::GetBoidStaticData(bool isPredator)
{
	if (isPredator)
		return s_predatorData;
	return s_preyData;
}

void NEATBoidLayer::addBoid(bool isPredator)
{
	chcl::Vector2<> position = m_simSpaceConstraint.clamp({ float(std::rand()), float(std::rand()) });
	float angle = chcl::radianSolver.clamp(float(std::rand()));
	m_boids.push_back(Boid(position, angle, isPredator));
}

void NEATBoidLayer::addBoid(unsigned int parent)
{
	m_boids.push_back(m_boids[parent].createOffspring());
}

void NEATBoidLayer::removeBoid(unsigned int boid)
{
	m_boids.erase(m_boids.begin() + boid);
}

void NEATBoidLayer::createPopulation(unsigned int passiveCount, unsigned int predatorCount)
{
	m_boids.reserve(passiveCount + predatorCount);

	for (unsigned int i = 0; i < passiveCount; ++i)
		addBoid(false);
	for (unsigned int i = 0; i < predatorCount; ++i)
		addBoid(true);
}

void NEATBoidLayer::Wanderer::init(chcl::Vector2<float> position)
{
	this->position = position;
	m_velocity = chcl::Vector2<float>::FromAngle(float(std::rand()));
	m_wanderAngle = 0;
	m_wanderOffset = 1.7f;
	m_wanderRadius = 0.5f;
	m_wanderMagnitude = 0.3f;
	m_maxSpeed = NEATBoidLayer::s_preyData.maxSpeed * 0.9f;
	m_wanderForce = 0.01f;
}

void NEATBoidLayer::Wanderer::wander()
{
	position += m_velocity;

	chcl::Vector2<float> offsetVec = m_velocity;
	offsetVec.setMagnitude(m_wanderOffset);
	chcl::Vector2<float> wanderVec = chcl::Vector2<float>::FromAngle(m_velocity.arg() + m_wanderAngle) * m_wanderRadius;
	chcl::Vector2<float> wanderSum = offsetVec + wanderVec;
	wanderSum.setMagnitude(m_wanderForce);

	m_velocity += wanderSum;
	if (m_velocity.mag() > m_maxSpeed)
		m_velocity.setMagnitude(m_maxSpeed);
	
	chcl::Vector2<float> wanderOffsetVec = chcl::Vector2<float>::FromAngle(float(std::rand()) / RAND_MAX * 2 * chcl::PI) * m_wanderMagnitude;
	m_wanderAngle += (chcl::Vector2<float>(m_wanderOffset, 0.f) + wanderOffsetVec).arg();
}