#pragma once

#include <vector>

#include <Nest.h>
#include <Nest/Events/KeyboardEvent.h>

#include <dataStorage/QuadTree.h>
#include <geometry/Rect.h>
#include <maths/Continuous.h>

#include "NEATNet.h"

class NEATBoidLayer : public Nest::Layer
{
	using QTType = chcl::QuadTree<unsigned int, 1>;
public:
	NEATBoidLayer(chcl::Rect simArea);

	void onAttach() override;
	void onDetach() override;
	void onUpdate(std::chrono::milliseconds timestep) override;
	void onEvent(Nest::Event &e) override;

private:
	struct BoidData
	{
		float viewDistance;
		float maxSpeed;
		float turnSpeed;
		float acceleration;
		float size;
		unsigned int growTime;
		float reproductionThreshold;
		NEATNet::MutationProbs mutationProbs;
	};

	struct Boid
	{
		chcl::Vector2<float> position;
		float angle, speed = 0.f;
		unsigned int age = 0;
		float reproductionTime = 0.f;
		bool isAlive = true;
		bool isPredator;
		NEATNet network;

		unsigned int closestPrey = 0, closestPredator = 0;

		Boid(chcl::Vector2<float> position, float angle, bool isPredator);

		Boid createOffspring() const;

		inline BoidData& getStaticData() const { return NEATBoidLayer::GetBoidStaticData(isPredator); }
	};

	static BoidData s_preyData, s_predatorData;

	std::vector<Boid> m_boids;
	QTType m_boidQT;

	class Wanderer
	{
	public:
		chcl::Vector2<float> position;

		void init(chcl::Vector2<float> position);
		void wander();

	private:
		chcl::Vector2<float> m_velocity;

		float m_wanderOffset, m_wanderRadius, m_wanderMagnitude;
		float m_maxSpeed, m_wanderForce;
		float m_wanderAngle;
	} m_boidTarget;

	chcl::Rect m_simArea;
	//chcl::Vector2<float> m_boidTarget, m_boidTargetDir;
	chcl::ContinuousSolver<chcl::Vector2<float>> m_simSpaceConstraint;

	Nest::Ref<Nest::Shader> m_boidShader;
	Nest::Ref<Nest::VertexArray> m_boidVertices;
	Nest::OrthographicCameraController m_camera;
	bool m_showBoidView = false;
	bool m_showQuadTree = false;

	bool isAdult(unsigned int boid) const;
	static BoidData& GetBoidStaticData(bool isPredator);

	void addBoid(bool isPredator);
	void addBoid(unsigned int parent);
	void removeBoid(unsigned int boid);
	void createPopulation(unsigned int passiveCount, unsigned int predatorCount);

	void updateBoidNetwork(unsigned int boid);
	void updateReproductionPassive(unsigned int boid);
	void updateReproductionPredator(unsigned int boid, unsigned int kills);
	void updateBoids();

	void drawBoids();
	void drawQTRegion(const QTType::QTRegion &qt);

	bool keyPressHandler(Nest::KeyPressedEvent &e);
};

