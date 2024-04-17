#include "NEATNet.h"

#include <cmath>

#include <Nest/Utils/Random.h>

std::vector<float(*)(float)> NEATNet::Node::nodeFuncs = {
	&NEATNet::Node::step,
	&NEATNet::Node::lin,
	&NEATNet::Node::linAbs,
	&NEATNet::Node::sigmoid
};

float NEATNet::Node::step(float val)
{
	if (val > 1.f)
		return 1;
	return 0;
}

float NEATNet::Node::lin(float val)
{
	if (val > 1.f)
		return 1;
	if (val < 0.f)
		return 0;
	return val;
}

float NEATNet::Node::linAbs(float val)
{
	if (abs(val) > 1.f)
		return 1;
	return abs(val);
}

float NEATNet::Node::sigmoid(float val)
{
	return 1 / (1 + std::exp(-val));
}

NEATNet::NEATNet(unsigned int m_inNodes, unsigned int m_outNodes, unsigned int initialConnections)
	: m_inNodes(m_inNodes), m_outNodes(m_outNodes)
{
	m_nodes.resize(numConstNodes());
	m_nodes[0].outValue = 1;	// Bias node added in first node slot

	for (unsigned int i = 0; i < initialConnections; ++i)
	{
		addRandomConnection();
	}
}

void NEATNet::setInputs(std::vector<float> inputs)
{
	for (unsigned int i = 0; i < m_inNodes; ++i)
	{
		// Skipping bias node
		m_nodes[i + 1].outValue = inputs[i];
	}
}

std::vector<float> NEATNet::getOutputs() const
{
	std::vector<float> result(m_outNodes);

	for (unsigned int i = 0; i < m_outNodes; ++i)
	{
		result[i] = m_nodes[m_inNodes + 1 + i].outValue;
	}

	return result;
}

void NEATNet::clear()
{
	// Remembering to skip over bias node
	for (unsigned int i = 1; i < m_nodes.size(); ++i)
	{
		m_nodes[i].outValue = 0;
	}
}

void NEATNet::process()
{
	for (const Connection &c : m_connections)
	{
		if (c.enabled)
			m_nodes[c.outNode].inValue += m_nodes[c.inNode].outValue * c.weight;
	}

	for (unsigned int i = m_inNodes + 1; i < m_nodes.size(); ++i)
	{
		m_nodes[i].outValue = m_nodes[i].nodeFunc(m_nodes[i].inValue);
		m_nodes[i].inValue = 0;
	}
}

NEATNet::Connection& NEATNet::addConnection(unsigned int inNode, unsigned int outNode, float weight)
{
	m_connections.push_back(Connection{ inNode, outNode, weight, true });
	return m_connections.back();
}

NEATNet::Node& NEATNet::addNode()
{
	m_nodes.push_back(Node());
	return m_nodes.back();
}

NEATNet::Node& NEATNet::addNode(unsigned int connection)
{
	m_nodes.push_back(Node());
	unsigned int newNode = m_nodes.size() - 1;

	m_connections[connection].enabled = false;
	addConnection(m_connections[connection].inNode, newNode, 1);
	addConnection(newNode, m_connections[connection].outNode, m_connections[connection].weight);
	return m_nodes.back();
}

void NEATNet::toggleConnection(unsigned int connection)
{
	m_connections[connection].enabled = !m_connections[connection].enabled;
}

void NEATNet::addRandomConnection()
{
	unsigned int inNode = Nest::Random::randomInt(0, numNodes() - m_outNodes);
	if (inNode > m_inNodes + 1)
		inNode += m_outNodes;
	unsigned int outNode = Nest::Random::randomInt(0, numNodes() - m_inNodes - 1) + m_inNodes + 1;
	float weight = Nest::Random::random(-1, 1);

	addConnection(inNode, outNode, weight);
}

void NEATNet::mutateConnectionWeight(unsigned int connection)
{
	float magnitude = Nest::Random::random(-1, 1);
	m_connections[connection].weight += (0.2f + abs(m_connections[connection].weight)) * magnitude;
}

void NEATNet::mutateConnectionEnabled(unsigned int connection)
{
	toggleConnection(connection);
}

void NEATNet::mutateNodeFunc(unsigned int node)
{
	m_nodes[node].nodeFunc = Node::nodeFuncs[Nest::Random::randomInt(0, Node::nodeFuncs.size())];
}

void NEATNet::mutate(const MutationProbs &probs)
{
	if (Nest::Random::random(0, 1) < probs.addConnectionChance)
		addRandomConnection();

	for (unsigned int i = 0; i < m_connections.size(); ++i)
	{
		if (Nest::Random::random(0, 1) < probs.removeConnectionChance)
		{
			removeConnection(i);
			--i;
			continue;
		}
		if (Nest::Random::random(0, 1) < probs.addNodeChance)
			addNode(i);
		if (Nest::Random::random(0, 1) < probs.changeWeightChance)
			mutateConnectionWeight(i);
		if (Nest::Random::random(0, 1) < probs.toggleConnectionChance)
			mutateConnectionEnabled(i);
	}

	for (unsigned int i = numConstNodes(); i < numNodes(); ++i)
	{
		if (Nest::Random::random(0, 1) < probs.removeNodeChance)
		{
			removeNode(i);
			trimNetwork();
			--i;
			continue;
		}
		if (Nest::Random::random(0, 1) < probs.changeNodeFuncChance)
			mutateNodeFunc(i);
	}

	trimNetwork();
}

void NEATNet::removeConnection(unsigned int connection)
{
	m_connections.erase(m_connections.begin() + connection);
}

void NEATNet::removeNode(unsigned int node)
{
	// Cannot remove input, output or bias nodes
	if (node < numConstNodes())
		return;

	// Remove all connections to/from node that will be removed
	std::erase_if(m_connections, [node](Connection c) {
		return c.inNode == node || c.outNode == node;
	});
	m_nodes.erase(m_nodes.begin() + node);
	// Every node further in the list than the removed node shift up one position
	// Since connections are based on node index, all connections to/from these nodes must be redirected to their new location
	for (Connection &c : m_connections)
	{
		if (c.inNode > node)
			--c.inNode;
		if (c.outNode > node)
			--c.outNode;
	}
}

void NEATNet::trimNetwork()
{
	auto looseNodes = getLooseNodes();
	while (!looseNodes.empty())
	{
		for (auto i = looseNodes.rbegin(); i != looseNodes.rend(); ++i)
		{
			// We do not want each node to trim the network, as it will get checked again later
			removeNode(*i);
		}

		looseNodes = getLooseNodes();
	}
}

NEATNet& NEATNet::operator=(const NEATNet &other)
{
	m_inNodes = other.m_inNodes;
	m_outNodes = other.m_outNodes;
	m_nodes = other.m_nodes;
	m_connections = other.m_connections;
	return *this;
}

std::vector<unsigned int> NEATNet::getLooseNodes() const
{
	std::vector<bool> hasInConnection(m_nodes.size(), false),
		hasOutConnection(m_nodes.size(), false);

	for (const Connection &c : m_connections)
	{
		hasInConnection[c.outNode] = true;
		hasOutConnection[c.inNode] = true;
	}

	std::vector<unsigned int> result;
	// Skip over input, output and bias nodes
	for (unsigned int i = numConstNodes(); i < m_nodes.size(); ++i)
	{
		if (!hasInConnection[i] || !hasOutConnection[i])
			result.push_back(i);
	}
	return result;
}
