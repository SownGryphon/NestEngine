#pragma once

#include <vector>

class NEATNet
{
	struct Node
	{
		float inValue = 0, outValue = 0;
		float(*nodeFunc)(float) = &sigmoid;

		static float step(float);
		static float lin(float);
		static float linAbs(float);
		static float sigmoid(float);

		static std::vector<float(*)(float)> nodeFuncs;
	};

	struct Connection
	{
		unsigned int inNode, outNode;
		float weight;
		bool enabled;
	};

public:
	struct MutationProbs
	{
		float addConnectionChance,
			addNodeChance,
			changeWeightChance,
			toggleConnectionChance,
			changeNodeFuncChance,
			removeConnectionChance,
			removeNodeChance;
	};

	NEATNet(unsigned int m_inNodes, unsigned int m_outNodes, unsigned int initialConnections);

	void setInputs(std::vector<float> inputs);
	std::vector<float> getOutputs() const;

	// Clears all node values
	void clear();
	// Propagates one update through the network
	void process();

	// Total number of nodes
	inline size_t numNodes() const { return m_nodes.size(); }
	// Number of nodes that are not input, output or bias nodes
	inline size_t numHiddenNodes() const { return numNodes() - numConstNodes(); }
	// Returns the total number of input, output and bias nodes
	inline size_t numConstNodes() const { return m_inNodes + m_outNodes + 1; }
	// Total number of connections
	inline size_t numConnections() const { return m_connections.size(); }

	void addRandomConnection();
	void mutateConnectionWeight(unsigned int connection);
	void mutateConnectionEnabled(unsigned int connection);
	void mutateNodeFunc(unsigned int node);

	void mutate(const MutationProbs &probs);

	Connection& addConnection(unsigned int inNode, unsigned int outNode, float weight);
	// Adds a new node to the nodes list
	Node& addNode();
	// Replaces an existing connection with a node and two connections
	Node& addNode(unsigned int connection);
	void toggleConnection(unsigned int connection);

	void removeConnection(unsigned int connection);
	void removeNode(unsigned int node);
	// Cleans up any nodes with no input or output connections
	void trimNetwork();

	NEATNet& operator=(const NEATNet& other);

private:
	unsigned int m_inNodes, m_outNodes;
	std::vector<Node> m_nodes;
	std::vector<Connection> m_connections;

	// Fins all nodes that have no input/output connections
	std::vector<unsigned int> getLooseNodes() const;
};