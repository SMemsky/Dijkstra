#include "GraphView.h"

#include <iostream>
#include <exception>
#include <stdexcept>

#include "Edge.h"
#include "GraphNode.h"
#include "GraphModel.h"
#include "GraphController.h"

GraphView::GraphView(GraphModel* model) :
	super(model),
	m_window(),
	m_model(model),
	m_nodeSize(25.0f),
	m_selectedNode(nullptr),
	m_captureMouse(true),
	m_font()
{}

GraphView::~GraphView()
{
	m_window.close();
}

void GraphView::updateNotify()
{}

void GraphView::init()
{
	m_window.create(sf::VideoMode(800, 600), "Dijkstra");
	m_window.setFramerateLimit(60);

	if (!m_font.loadFromFile("sansation.ttf"))
	{
		throw std::runtime_error("Unable to load font");
	}

	super::init();
}

void GraphView::drawNode(unsigned x, unsigned y, bool selected /* = false */)
{
	sf::Vector2f pos;
	pos.x = static_cast<float>(x) - 20.0f;
	pos.y = static_cast<float>(y) - 20.0f;

	sf::CircleShape node(m_nodeSize * 0.88f, 24);
	node.setFillColor(sf::Color(61, 48, 35));
	if (selected)
	{
		node.setOutlineColor(sf::Color(175, 132, 80));
	}
	else
	{
		node.setOutlineColor(sf::Color(101, 76, 46));
	}

	node.setOutlineThickness(m_nodeSize * 0.12f);
	node.setPosition(pos);

	m_window.draw(node);
}

void GraphView::drawNodeChar(unsigned x, unsigned y, char ch)
{
	sf::Text text(std::string(1, ch), m_font, 25.0f);
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	text.setPosition(sf::Vector2f(x, y));

	m_window.draw(text);
}

void GraphView::drawEdge(const sf::Vector2f& nodeA, const sf::Vector2f& nodeB,
	bool selected /* = false */)
{
	//std::cout << "Draw edge!" << std::endl;
	sf::Vector2f dirA = nodeB - nodeA;
	sf::Vector2f dirB = nodeA - nodeB;

	// Normalize dir
	float mag = 1.0f / std::sqrt(dirA.x * dirA.x + dirA.y * dirA.y);
	dirA.x *= mag;
	dirA.y *= mag;
	mag = 1.0f / std::sqrt(dirB.x * dirB.x + dirB.y * dirB.y);
	dirB.x *= mag;
	dirB.y *= mag;

	sf::Vector2f begin = nodeA + (dirA * m_nodeSize);
	sf::Vector2f end = nodeB + (dirB * m_nodeSize);

	Edge edge(begin, end, selected);
	m_window.draw(edge);
}

void GraphView::drawEdgeWeight(const sf::Vector2f& begin,
	const sf::Vector2f& end, unsigned weight)
{
	sf::Text text(std::to_string(weight), m_font, 12.0f);
	sf::Vector2f diff = end - begin;
	text.setRotation(std::atan(diff.y / diff.x) * (180.0f / M_PI));
	sf::Vector2f pos = begin + diff / 2.0f;
	pos += text.getTransform().transformPoint(
		sf::Vector2f(-(text.getLocalBounds().width / 2.0f), 0));
	text.setPosition(pos);

	m_window.draw(text);
}

void GraphView::draw()
{
	m_window.clear(sf::Color(125, 137, 71));

	GraphNode* b = m_model->getPathBegin();
	GraphNode* e = m_model->getPathEnd();

	auto& nodes = m_model->getGraphNodes();
	auto nodesEnd = nodes.end();
	for (auto nodesIt = nodes.begin(); nodesIt != nodesEnd; ++nodesIt)
	{
		auto& data = (*nodesIt)->getDataRef();
		if ((*nodesIt) == m_selectedNode)
		{
			drawNode(data.pos.x, data.pos.y, true);
		}
		else
		{
			drawNode(data.pos.x, data.pos.y);
		}

		if ((*nodesIt) == b)
		{
			drawNodeChar(data.pos.x, data.pos.y, 'B');
		}
		else if ((*nodesIt) == e)
		{
			drawNodeChar(data.pos.x, data.pos.y, 'E');
		}
	}

	GraphNode* source = nullptr;
	GraphNode* destination = nullptr;

	// Draw connections
	for (auto nodesIt = nodes.begin(); nodesIt != nodesEnd; ++nodesIt)
	{
		auto& edges = (*nodesIt)->getEdgeSet();
		auto edgesEnd = edges.end();
		for (auto edgesIt = edges.begin(); edgesIt != edgesEnd; ++edgesIt)
		{
			if ((*edgesIt)->getSource() == (*nodesIt))
			{
				source = (*edgesIt)->getSource();
				destination = (*edgesIt)->getDestination();
			}
			else if ((*edgesIt)->isTwoWay() == true)
			{
				source = (*edgesIt)->getDestination();
				destination = (*edgesIt)->getSource();
			}
			else
			{
				continue;
			}

			auto& sourceData = source->getDataRef();
			auto& destinationData = destination->getDataRef();

			bool highlight = false;
			if ((*edgesIt)->getSource() == (*nodesIt))
			{
				if ((*nodesIt)->getNextNode() == (*edgesIt)->getDestination())
				{
					highlight = true;
				}
			}
			else
			{
				if ((*nodesIt)->getNextNode() == (*edgesIt)->getSource())
				{
					highlight = true;
				}
			}

			drawEdge(sf::Vector2f(sourceData.pos.x, sourceData.pos.y),
				sf::Vector2f(destinationData.pos.x, destinationData.pos.y),
				highlight);
			drawEdgeWeight(sf::Vector2f(sourceData.pos.x, sourceData.pos.y),
				sf::Vector2f(destinationData.pos.x, destinationData.pos.y),
				(*edgesIt)->getWeight());
		}

		if ((*nodesIt) == m_selectedNode &&
			m_captureMouse == true)
		{
			sf::Vector2i pos = sf::Mouse::getPosition(m_window);
			auto& data = (*nodesIt)->getDataRef();
			drawEdge(sf::Vector2f(data.pos.x, data.pos.y),
				sf::Vector2f(pos.x, pos.y),
				false);
		}
	}

	//std::cout << "End frame" << std::endl;

	m_window.display();
}

GraphModel* GraphView::getModel()
{
	return m_model;
}

sf::RenderWindow* GraphView::getWindow()
{
	return &m_window;
}

Controller* GraphView::makeController()
{
	return new GraphController(this);
}

void GraphView::setSelectedNode(GraphNode* node)
{
	m_selectedNode = node;
}

GraphNode* GraphView::getSelectedNode() const
{
	return m_selectedNode;
}

void GraphView::setCaptureMouse(bool value)
{
	m_captureMouse = value;
}

bool GraphView::getCaptureMouse() const
{
	return m_captureMouse;
}
