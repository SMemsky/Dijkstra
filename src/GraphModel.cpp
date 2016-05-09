#include "GraphModel.h"

#include <iostream>

#include "Graph.h"

GraphModel::GraphModel() :
	super(),
	m_graph(nullptr)
{
	m_graph = new Graph();
}

GraphModel::~GraphModel()
{
	delete m_graph;
	m_graph = nullptr;
}

const std::set<GraphNode*>& GraphModel::getGraphNodes() const
{
	return m_graph->getNodes();
}

GraphNode* GraphModel::addNode(const GraphNodeData& data)
{
	return m_graph->addNode(data);
}

void GraphModel::removeNode(GraphNode* node)
{
	m_graph->removeNode(node);
}

void GraphModel::addEdge(GraphNode* source, GraphNode* destination, unsigned weight)
{
	m_graph->addEdge(source, destination, weight);
}

void GraphModel::removeEdge(GraphNode* source, GraphNode* destination)
{
	m_graph->removeEdge(source, destination);
}

void GraphModel::setPathBegin(GraphNode* begin)
{
	m_graph->setPathBegin(begin);
}

void GraphModel::setPathEnd(GraphNode* end)
{
	m_graph->setPathEnd(end);
}

GraphNode* GraphModel::getPathBegin() const
{
	return m_graph->getPathBegin();
}

GraphNode* GraphModel::getPathEnd() const
{
	return m_graph->getPathEnd();
}

