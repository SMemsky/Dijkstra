#pragma once

#include <set>

#include "Model.h"

class Graph;
class GraphNode;
class GraphNodeData;

class GraphModel : public Model
{
	typedef Model super;
public:
	GraphModel();
	~GraphModel();

	const std::set<GraphNode*>& getGraphNodes() const;
	GraphNode* addNode(const GraphNodeData& data);
	void removeNode(GraphNode* node);

	void addEdge(GraphNode* source, GraphNode* destination, unsigned weight);
	void removeEdge(GraphNode* source, GraphNode* destination);

	void setPathBegin(GraphNode* begin);
	void setPathEnd(GraphNode* end);
	GraphNode* getPathBegin() const;
	GraphNode* getPathEnd() const;
private:
	Graph* m_graph;
};
