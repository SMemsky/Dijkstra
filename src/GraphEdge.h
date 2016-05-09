#pragma once

class Graph;
class GraphNode;

class GraphEdge
{
	friend class Graph;
public:
	GraphEdge(GraphNode* source, GraphNode* destination, unsigned weight) :
		m_source(source),
		m_destination(destination),
		m_weight(weight),
		m_twoWay(false)
	{}

	GraphNode* getSource() const
	{
		return m_source;
	}

	GraphNode* getDestination() const
	{
		return m_destination;
	}

	unsigned getWeight() const
	{
		return m_weight;
	}

	bool isTwoWay() const
	{
		return m_twoWay;
	}
private:
	GraphNode* m_source;
	GraphNode* m_destination;
	unsigned m_weight;
	bool m_twoWay;
};
