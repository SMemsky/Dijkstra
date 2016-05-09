#pragma once

#include <set>

#include "GraphEdge.h"
#include "GraphNodeData.h"

class Graph;

class GraphNode
{
	friend class Graph;
public:
	GraphNode(const GraphNodeData& data) :
		m_edges(),
		m_data(data),
		m_nextNode(nullptr),
		m_previousNode(nullptr),
		m_visited(false),
		m_tentativeDistance(-1)
	{}

	const std::set<GraphEdge*>& getEdgeSet() const
	{
		return m_edges;
	}

	GraphNodeData& getDataRef()
	{
		return m_data;
	}

	GraphNode* getNextNode() const
	{
		return m_nextNode;
	}

	GraphNode* getPreviousNode() const
	{
		return m_previousNode;
	}
private:
	std::set<GraphEdge*> m_edges;
	GraphNodeData m_data;

	GraphNode* m_nextNode;
	GraphNode* m_previousNode;

	// Dijkstra's algorithm implementation specific
	bool m_visited;
	unsigned m_tentativeDistance;
};
