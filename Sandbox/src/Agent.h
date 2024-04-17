#pragma once

#include <geometry/Vector2.h>
#include <geometry/Rect.h>
#include <maths/Continuous.h>

#include "NEATNet.h"

class Agent
{
public:
	struct Genome
	{
		float size;
		float maxSpeed;
		float turnSpeed;				// Radians / tick
		float acceleration;
		float viewRadius;
		float reproductionThreshold;	// Energy required for reproduction
		float childEnergy;				// Proportion of energy to give to children
		unsigned int growTime;
	};

	chcl::Vector2<float> position;
	float direction, speed;
	float energy;
	unsigned int age;
	bool aggressive = false;
	bool alive = true;
	Genome genome;
	NEATNet net;

	unsigned int bloodline;

	Agent(float x, float y, float direction, float energy);
	Agent(chcl::Vector2<float> position, float direction, float energy);

	void updateNetwork(chcl::Vector2<float> closestFood, const Agent &closestAgent);
	void update();

	Agent createChild();

	inline bool isAdult() const { return age > genome.growTime; }

	struct EnergyCosts {
		float moveCost;			// Proportional to size * speed^2
		float turnCost;			// Proportional to size
		float accelerateCost;	// Proportional to size
		float viewCost;			// Proportional to viewRadius^2
		float liveCost;			// Proportional to size^2
	};

	inline static void SetEnergyCosts(EnergyCosts &costs) { s_energyCosts = costs; }
	static void SetBounds(chcl::Rect bounds);

private:
	static EnergyCosts s_energyCosts;
	static NEATNet::MutationProbs s_netMutationProbs;
	static chcl::ContinuousSolver<chcl::Vector2<float>> s_boundsConstraint;

	void mutateGenome();
};

