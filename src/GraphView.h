#pragma once

#include <SFML/Graphics.hpp>

#include "View.h"

enum MouseMode
{
	MOUSE_NORMAL,
	MOUSE_SELECT,
	MOUSE_DRAG,
	MOUSE_
};

class GraphModel;
class GraphNode;

class GraphView : public View
{
	typedef View super;
public:
	GraphView(GraphModel* model);
	virtual ~GraphView();

	virtual void updateNotify() override;

	virtual void init() override;
	virtual void draw() override;

	GraphModel* getModel(); // Does not override getModel from base class
	sf::RenderWindow* getWindow();

	void setSelectedNode(GraphNode* node);
	GraphNode* getSelectedNode() const;

	void setCaptureMouse(bool value);
	bool getCaptureMouse() const;
private:
	Controller* makeController() override;

	void drawNode(unsigned x, unsigned y, bool selected = false);
	void drawNodeChar(unsigned x, unsigned y, char ch);
	void drawEdge(const sf::Vector2f& nodeA, const sf::Vector2f& nodeB,
		bool selected = false);
	void drawEdgeWeight(const sf::Vector2f& begin, const sf::Vector2f& end,
		unsigned weight);
private:
	sf::RenderWindow m_window;
	GraphModel* m_model; // Does not override m_model from base class

	const float m_nodeSize;

	GraphNode* m_selectedNode;
	bool m_captureMouse;

	sf::Font m_font;
};
