#pragma once

#include <vector>

#include <dataStorage/QuadTree.h>
#include <geometry/Rect.h>

#include <Nest.h>

#include "Agent.h"

class EvolutionSimLayer : public Nest::Layer
{
public:
	EvolutionSimLayer(float simAreaWidth, float simAreaHeight, float aspectRatio);

	virtual void onAttach() override;
	virtual void onDetach() override;
	virtual void onUpdate(std::chrono::milliseconds timestep) override;
	virtual void onEvent(Nest::Event &e) override;

private:
	struct Food
	{
		chcl::Vector2<float> position;
		bool eaten = false;

		static float s_foodEnergy;
	};

	bool onKeyEvent(Nest::KeyPressedEvent &e);

	void initPopulation(unsigned int agentCount, unsigned int foodCount);

	void drawAgent(const Agent &agent);

	chcl::Rect m_simArea;

	Nest::Ref<Nest::Shader> m_agentShader;
	Nest::Ref<Nest::VertexArray> m_agentVertices;
	Nest::OrthographicCameraController m_camera;
	bool m_showBoidView = false;
	bool m_showQuadTree = false;

	std::vector<Agent> m_agents;
	std::vector<Food> m_foods;

	chcl::QuadTree<Agent*, 1> m_agentQT;
	chcl::QuadTree<Food*, 1> m_foodQT;
};

