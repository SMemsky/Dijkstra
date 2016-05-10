#include "Graph.h"

#include <functional>
#include <exception>
#include <stdexcept>
#include <utility>
#include <cassert>
#include <limits>

Graph::Graph() :
	m_nodes(),
	m_pathBegin(nullptr),
	m_pathEnd(nullptr),
	m_needsUpdate(false)
{}

Graph::~Graph()
{
	while (!m_nodes.empty())
	{
		removeNode(*(m_nodes.begin()));
	}
}

GraphNode* Graph::addNode(const GraphNodeData& data)
{
	auto newNode = new GraphNode(data);
	if (newNode == nullptr)
	{
		throw std::runtime_error("Unable to create GraphNode");
	}

	m_nodes.insert(newNode);

	m_needsUpdate = true;

	return newNode;
}

void Graph::removeNode(GraphNode* node)
{
	if (!isInGraph(node))
	{
		throw std::runtime_error("The node does not belong to this graph");
	}

	if (m_pathBegin == node)
	{
		m_pathBegin = nullptr;
	}
	else if (m_pathEnd == node)
	{
		m_pathEnd = nullptr;
	}

	m_needsUpdate = true;

	// Do i really need this lambda here?
	//auto
	std::function<void(GraphEdge*)> eraseEdge = [node](GraphEdge* edge)
	{
		if (edge->m_source == node)
		{
			edge->m_destination->m_edges.erase(edge);
		}
		else
		{
			edge->m_source->m_edges.erase(edge);
		}

		node->m_edges.erase(edge);
		delete edge;
	};

	while (!node->m_edges.empty())
	{
		eraseEdge(*(node->m_edges.begin()));
	}

	m_nodes.erase(node);
	delete node;
}

const std::set<GraphNode*>& Graph::getNodes() const
{
	if (m_needsUpdate)
	{
		computePaths();
	}

	return m_nodes;
}

void Graph::addEdge(GraphNode* source, GraphNode* destination, unsigned weight)
{
	assert(source != destination);

	if (!isInGraph(source))
	{
		throw std::runtime_error(
			"The source node does not belong to this graph");
	}
	if (!isInGraph(destination))
	{
		throw std::runtime_error(
			"The destination node does not belong to this graph");
	}

	m_needsUpdate = true;

	auto end = source->m_edges.end();
	for (auto it = source->m_edges.begin(); it != end; ++it)
	{
		if ((*it)->m_destination == destination)
		{
			// Edge is already created
			return;
		}
		else if ((*it)->m_source == destination)
		{
			// Destination node is connected to source node

			if ((*it)->m_twoWay == false)
			{
				// Make edge two-way
				(*it)->m_twoWay = true;
			}

			return;
		}
	}

	GraphEdge* newEdge = new GraphEdge(source, destination, weight);
	source->m_edges.insert(newEdge);
	destination->m_edges.insert(newEdge);
}

void Graph::removeEdge(GraphNode* source, GraphNode* destination)
{
	assert(source != destination);

	if (!isInGraph(source))
	{
		throw std::runtime_error(
			"The source node does not belong to this graph");
	}
	if (!isInGraph(destination))
	{
		throw std::runtime_error(
			"The destination node does not belong to this graph");
	}

	m_needsUpdate = true;

	auto end = source->m_edges.end();
	for (auto it = source->m_edges.begin(); it != end; ++it)
	{
		if ((*it)->m_destination == destination)
		{
			if ((*it)->m_twoWay == true)
			{
				std::swap((*it)->m_destination, (*it)->m_source);
				(*it)->m_twoWay = false;
			}
			else
			{
				// Delete edge while iterator is still valid
				delete *it;
				destination->m_edges.erase(*it);
				source->m_edges.erase(*it);
			}

			return;
		}
		else if ((*it)->m_source == destination)
		{
			if ((*it)->m_twoWay == true)
			{
				(*it)->m_twoWay = false;
			}
			else
			{
				// Keeping iterator alive...
				delete *it;
				destination->m_edges.erase(*it);
				source->m_edges.erase(*it);
			}

			return;
		}
	}

	throw std::runtime_error("Unable to find edge!");
}

void Graph::setPathBegin(GraphNode* begin)
{
	if (begin != nullptr &&
		!isInGraph(begin))
	{
		throw std::runtime_error("The node does not belong to this graph");
	}
	if (m_pathBegin == begin)
	{
		return;
	}

	m_needsUpdate = true;

	m_pathBegin = begin;
}

void Graph::setPathEnd(GraphNode* end)
{
	if (end != nullptr &&
		!isInGraph(end))
	{
		throw std::runtime_error("The node does not belong to this graph");
	}
	if (m_pathEnd == end)
	{
		return;
	}

	m_needsUpdate = true;

	m_pathEnd = end;
}

GraphNode* Graph::getPathBegin() const
{
	return m_pathBegin;
}

GraphNode* Graph::getPathEnd() const
{
	return m_pathEnd;
}

bool Graph::isInGraph(const GraphNode* node) const
{
	auto end = m_nodes.end();
	for (auto it = m_nodes.begin(); it != end; ++it)
	{
		if ((*it) == node)
		{
			return true;
		}
	}

	return false;
}

void Graph::computePaths() const
{
	m_needsUpdate = false;

	if (m_pathBegin == nullptr ||
		m_pathEnd == nullptr)
	{
		return;
	}

	auto end = m_nodes.end();
	for (auto it = m_nodes.begin(); it != end; ++it)
	{
		(*it)->m_visited = false;
		(*it)->m_tentativeDistance = std::numeric_limits<unsigned>::max();
		(*it)->m_previousNode = nullptr;
		(*it)->m_nextNode = nullptr;
	}

	GraphNode* current = m_pathBegin;
	current->m_tentativeDistance = 0;
	while (true)
	{
		auto edgesEnd = current->m_edges.end();
		for (auto it = current->m_edges.begin(); it != edgesEnd; ++it)
		{
			GraphNode* destination = nullptr;
			if ((*it)->m_source == current)
			{
				destination = (*it)->m_destination;
			}
			else if ((*it)->m_destination == current &&
				(*it)->m_twoWay == true)
			{
				destination = (*it)->m_source;
			}

			if (destination == nullptr ||
				destination->m_visited)
			{
				continue;
			}

			unsigned sum = current->m_tentativeDistance + (*it)->m_weight;
			if (destination->m_tentativeDistance > sum)
			{
				destination->m_tentativeDistance = sum;
				destination->m_previousNode = current;
			}
		}

		current->m_visited = true;

		GraphNode* lastNode = nullptr;
		unsigned lastDistance = std::numeric_limits<unsigned>::max();
		auto nodesEnd = m_nodes.end();
		for (auto it = m_nodes.begin(); it != nodesEnd; ++it)
		{
			if ((*it)->m_visited)
			{
				continue;
			}

			if ((*it)->m_tentativeDistance < lastDistance)
			{
				lastNode = *it;
				lastDistance = lastNode->m_tentativeDistance;
			}
		}

		// No more unvisited nodes connected to start point -> exit loop
		if (lastNode == nullptr)
		{
			break;
		}

		current = lastNode;
	}

	std::function<void(GraphNode*)> completePath = [&](GraphNode* node)
	{
		if (node->m_previousNode != nullptr)
		{
			node->m_previousNode->m_nextNode = node;
			completePath(node->m_previousNode);
		}
	};

	completePath(m_pathEnd);
}
