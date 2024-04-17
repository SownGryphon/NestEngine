#include "Agent.h"

#include <maths/Common.h>
#include <maths/Continuous.h>
#include <maths/SquareMatrix.h>

#include <Nest/Utils/Random.h>

NEATNet::MutationProbs Agent::s_netMutationProbs;
Agent::EnergyCosts Agent::s_energyCosts;
chcl::ContinuousSolver<chcl::Vector2<float>> Agent::s_boundsConstraint(0.f, 0.f);

Agent::Agent(float x, float y, float direction, float energy)
	: position(x, y),
	direction(direction), speed(0.f),
	energy(energy),
	age(0),
	net(8, 5, 5)
{
	genome.size = Nest::Random::random(5.f, 15.f);
	genome.maxSpeed = Nest::Random::random(0.5f, 2.f);
	genome.turnSpeed = chcl::toRadians(Nest::Random::random(1.f, 3.f));
	genome.acceleration = Nest::Random::random(0.01f, 0.05f);
	genome.viewRadius = Nest::Random::random(50.f, 150.f);
	genome.reproductionThreshold = Nest::Random::random(energy / 2, energy * 3);
	genome.childEnergy = Nest::Random::random(0.01f, 1.f);
	genome.growTime = Nest::Random::randomInt(50, 300);

	s_netMutationProbs.addConnectionChance = 0.15f;
	s_netMutationProbs.addNodeChance = 0.03f;
	s_netMutationProbs.changeWeightChance = 0.6f;
	s_netMutationProbs.changeNodeFuncChance = 0.2f;
	s_netMutationProbs.toggleConnectionChance = 0.25f;
	s_netMutationProbs.removeConnectionChance = 0.05f;
	s_netMutationProbs.removeNodeChance = 0.01f;	// Removing a node also removes all of its connections, so this chance should be low

	static unsigned int bloodlineCounter = 0;
	this->bloodline = bloodlineCounter;
	++bloodlineCounter;
}

Agent::Agent(chcl::Vector2<float> position, float direction, float energy)
	: Agent(position.x, position.y, direction, energy)
{
}

void Agent::updateNetwork(chcl::Vector2<float> closestFood, const Agent &closestAgent)
{
	chcl::Mat2 rotationMat = chcl::Mat2::Rotation(-direction);
	chcl::Vector2<float> foodRelative = rotationMat * (closestFood - position),
		agentRelative = rotationMat * (closestAgent.position - position);

	// Network inputs:
	// - Food forward distance - [-1, 1] (0 if none)
	// - Food sideways distance - [-1, 1] (0 if none)
	// - Agent forward distance - [-1, 1] (0 if none)
	// - Agent sideways distance - [-1, 1] (0 if none)
	// - Agent relative direction - [-1, 1] (0 if none)
	// - Agent size - log2(agent size / own size) (0 if none)
	// - Agent speed - agent speed / own max speed (1 if none)
	// - Own speed - [0, 1]
	std::vector<float> inputs = {
		foodRelative.x / genome.viewRadius,
		foodRelative.y / genome.viewRadius,
		agentRelative.x / genome.viewRadius,
		agentRelative.y / genome.viewRadius,
		chcl::radianSolver.diffMin(direction, closestAgent.direction) / chcl::PI,
		std::log2f(closestAgent.genome.size / genome.size),
		closestAgent.speed / genome.maxSpeed,
		speed / genome.maxSpeed
	};

	net.setInputs(inputs);
}

void Agent::update()
{
	if (energy <= 0.f)
		alive = false;

	++age;
	if (!isAdult())
		return;

	// Network processing
	net.process();
	auto outputs = net.getOutputs();
	float turnAmount = (outputs[0] - outputs[1]) * genome.turnSpeed;
	float accelerationAmount = (outputs[2] - outputs[3]) * genome.acceleration;

	// Property updates
	direction += turnAmount;
	speed += accelerationAmount;
	chcl::Vector2<float> forward = chcl::Vector2<float>::FromAngle(direction);
	position += forward * speed;
	aggressive = outputs[4] > 0.5f;

	// Property clamping
	direction = chcl::radianSolver.clamp(direction);
	if (speed < 0)
		speed = 0;
	if (speed > genome.maxSpeed)
		speed = genome.maxSpeed;
	position = s_boundsConstraint.clamp(position);

	// Energy updates
	energy -= std::abs(turnAmount) * genome.size * s_energyCosts.turnCost;
	energy -= std::abs(accelerationAmount) * genome.size * s_energyCosts.accelerateCost;
	energy -= speed * genome.size * std::powf(speed, 2) * s_energyCosts.moveCost;
	energy -= std::powf(genome.viewRadius, 2) * s_energyCosts.viewCost;
	energy -= std::powf(genome.size, 2) * s_energyCosts.liveCost;
}

Agent Agent::createChild()
{
	Agent child = *this;
	child.age = 0;
	child.speed = 0;
	child.energy = energy * genome.childEnergy;
	energy *= 1 - genome.childEnergy;

	child.net.mutate(s_netMutationProbs);
	child.mutateGenome();
	
	return child;
}

void Agent::mutateGenome()
{
	static float mutationMagnitude = 0.05f;

	genome.size *= Nest::Random::randomGeometric(1 + mutationMagnitude);
	genome.maxSpeed *= Nest::Random::randomGeometric(1 + mutationMagnitude);
	genome.turnSpeed *= Nest::Random::randomGeometric(1 + mutationMagnitude);
	genome.acceleration *= Nest::Random::randomGeometric(1 + mutationMagnitude);
	genome.viewRadius *= Nest::Random::randomGeometric(1 + mutationMagnitude);
	genome.reproductionThreshold *= Nest::Random::randomGeometric(1 + mutationMagnitude);
	genome.growTime *= Nest::Random::randomGeometric(1 + mutationMagnitude);

	genome.childEnergy += Nest::Random::random(-mutationMagnitude, mutationMagnitude);
	if (genome.childEnergy < 0.01f)
		genome.childEnergy = 0.01f;
	else if (genome.childEnergy > 1.f)
		genome.childEnergy = 1.f;
}

void Agent::SetBounds(chcl::Rect bounds)
{
	s_boundsConstraint = chcl::ContinuousSolver<chcl::Vector2<float>>(bounds.bl(), bounds.tr());
}