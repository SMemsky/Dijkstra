#include "GraphController.h"

#include <iostream>
#include <cmath>

#include "GraphView.h"
#include "GraphNode.h"
#include "GraphModel.h"

GraphController::GraphController(GraphView* view) :
	super(view),
	m_deltaClock(nullptr),
	m_deltaTime(0.0f),
	m_window(view->getWindow()),
	m_model(view->getModel()),
	m_view(view),
	m_mouseClick{{0, 0}, 1.0f, false}
{}

GraphController::~GraphController()
{
	if (m_deltaClock != nullptr)
	{
		delete m_deltaClock;
	}
}

void GraphController::updateNotify()
{}

bool GraphController::update()
{
	if (m_deltaClock == nullptr)
	{
		m_deltaClock = new sf::Clock();
		m_deltaClock->restart();
	}

	m_deltaTime = m_deltaClock->restart().asSeconds();

	sf::Event event;
	while (m_window->pollEvent(event))
	{
		if (!handleEvent(event))
		{
			return false;
		}
	}

	if (m_mouseClick.sinceSingleClick < 0.5f)
	{
		m_mouseClick.sinceSingleClick += m_deltaTime;
	}

	m_view->draw();

	return true;
}

float GraphController::distance(unsigned ax, unsigned ay,
	unsigned bx, unsigned by) const
{
	float dx = static_cast<int>(bx) - static_cast<int>(ax);
	float dy = static_cast<int>(by) - static_cast<int>(ay);

	return std::sqrt(dx*dx + dy*dy);
}

GraphNode* GraphController::findNodeAt(unsigned x, unsigned y) const
{
	float radius = 25.0f;
	auto& nodes = m_model->getGraphNodes();
	auto end = nodes.rend();
	for (auto it = nodes.rbegin(); it != end; ++it)
	{
		auto& data = (*it)->getDataRef();
		if (distance(x, y, data.pos.x, data.pos.y) < radius)
		{
			return (*it);
		}
	}

	return nullptr;
}

GraphEdge* GraphController::findEdge(GraphNode* source, GraphNode* destination)
	const
{
	auto& edges = source->getEdgeSet();
	auto end = edges.end();
	for (auto it = edges.begin(); it != end; ++it)
	{
		if ((*it)->getDestination() == destination)
		{
			return (*it);
		}
		else if ((*it)->getSource() == destination &&
			(*it)->isTwoWay())
		{
			return (*it);
		}
	}

	return nullptr;
}

void GraphController::handleLeftClick(int x, int y)
{
	if (x < 0 ||
		y < 0)
	{
		return;
	}

	GraphNode* nodeFound = findNodeAt(x, y);
	if (nodeFound == nullptr)
	{
		// Deselect node
		m_view->setSelectedNode(nullptr);
		m_view->setCaptureMouse(false);
	}
	else
	{
		m_mouseClick.sinceSingleClick = 0.0f;
		m_mouseClick.isDoubleClick = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		{
			if (m_model->getPathEnd() == nodeFound)
			{
				m_model->setPathEnd(nullptr);
			}

			if (m_model->getPathBegin() == nodeFound)
			{
				m_model->setPathBegin(nullptr);
			}
			else
			{
				m_model->setPathBegin(nodeFound);
			}
		}
		else
		{
			m_view->setSelectedNode(nodeFound);
			m_view->setCaptureMouse(true);
		}
	}

	if (m_mouseClick.sinceSingleClick < 0.5f)
	{
		// Double click
		m_mouseClick.isDoubleClick = true;
		
		if (nodeFound == nullptr &&
			distance(
				m_mouseClick.lastPos.x, m_mouseClick.lastPos.y, x, y) < 5.0f)
		{
			m_model->addNode(GraphNodeData(
				static_cast<unsigned>(x), static_cast<unsigned>(y)));
		}
	}
	else
	{
		// Single click
		m_mouseClick.sinceSingleClick = 0.0f;
		m_mouseClick.isDoubleClick = false;
	}

	m_mouseClick.lastPos.x = static_cast<unsigned>(x);
	m_mouseClick.lastPos.y = static_cast<unsigned>(y);
}

void GraphController::handleLeftRelease(int x, int y)
{
	if (x < 0 ||
		y < 0)
	{
		m_view->setCaptureMouse(false);
		return;
	}

	GraphNode* nodeFound = findNodeAt(x, y);
	if (nodeFound != nullptr &&
		m_view->getCaptureMouse() &&
		m_view->getSelectedNode() != nodeFound)
	{
		GraphNode* source = m_view->getSelectedNode();
		GraphNode* destination = nodeFound;

		auto& sourceData = source->getDataRef();
		auto& destinationData = destination->getDataRef();

		GraphEdge* edge = findEdge(source, destination);
		if (edge == nullptr)
		{
			//edge = findEdge(destination, source);
		}

		if (edge == nullptr)
		{
			m_model->addEdge(source, destination,
			static_cast<unsigned>(distance(sourceData.pos.x, sourceData.pos.y,
				destinationData.pos.x, destinationData.pos.y)));
		}
		else
		{
			m_model->removeEdge(source, destination);
		}
	}

	m_view->setCaptureMouse(false);
}

void GraphController::handleRightClick(int x, int y)
{
	if (x < 0 ||
		y < 0)
	{
		return;
	}

	GraphNode* nodeFound = findNodeAt(x, y);
	if (nodeFound == nullptr)
	{
		// Deselect node
	}
	else
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		{
			if (m_model->getPathBegin() == nodeFound)
			{
				m_model->setPathBegin(nullptr);
			}
			
			if (m_model->getPathEnd() == nodeFound)
			{
				m_model->setPathEnd(nullptr);
			}
			else
			{
				m_model->setPathEnd(nodeFound);
			}
		}
		else
		{
			m_model->removeNode(nodeFound);
		}
	}

	// Deselect anyway
	m_view->setSelectedNode(nullptr);
}

bool GraphController::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		return false;
	}
	else if (event.type == sf::Event::KeyPressed)
	{

	}
	else if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			handleLeftClick(event.mouseButton.x, event.mouseButton.y);
		}
		else if (event.mouseButton.button == sf::Mouse::Right)
		{
			handleRightClick(event.mouseButton.x, event.mouseButton.y);
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			handleLeftRelease(event.mouseButton.x, event.mouseButton.y);
		}
	}

	return true;
}
