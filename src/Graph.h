#pragma once

#include <set>

#include "GraphNode.h"

class Graph
{
public:
	Graph();
	~Graph();

	GraphNode* addNode(const GraphNodeData& data);
	void removeNode(GraphNode* node);
	const std::set<GraphNode*>& getNodes() const;

	void addEdge(GraphNode* source, GraphNode* destination, unsigned weight);
	void removeEdge(GraphNode* source, GraphNode* destination);

	void setPathBegin(GraphNode* begin);
	void setPathEnd(GraphNode* end);
	GraphNode* getPathBegin() const;
	GraphNode* getPathEnd() const;
private:
	// Checks that node belongs to this graph
	bool isInGraph(const GraphNode* node) const;
	void computePaths() const;
private:
	std::set<GraphNode*> m_nodes;
	GraphNode* m_pathBegin;
	GraphNode* m_pathEnd;

	mutable bool m_needsUpdate;
};
