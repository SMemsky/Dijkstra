#pragma once

#include <SFML/Graphics.hpp>

#include "Controller.h"

class GraphView;
class GraphNode;
class GraphModel;

class GraphController : public Controller
{
	typedef Controller super;
public:
	GraphController(GraphView* view);
	~GraphController();

	void updateNotify() override;

	bool update() override; // Actual update function
private:
	bool handleEvent(const sf::Event& event);
	void handleLeftClick(int x, int y);
	void handleLeftRelease(int x, int y);
	void handleRightClick(int x, int y);

	GraphNode* findNodeAt(unsigned x, unsigned y) const;
	float distance(unsigned ax, unsigned ay, unsigned bx, unsigned by) const;
private:
	sf::Clock* m_deltaClock;
	float m_deltaTime;
	sf::RenderWindow* m_window;
	GraphModel* m_model;
	GraphView* m_view;

	struct
	{
		struct
		{
			unsigned x;
			unsigned y;
		} lastPos;

		float sinceSingleClick;
		bool isDoubleClick;
	} m_mouseClick;
};
