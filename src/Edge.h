#pragma once

#include <SFML/Graphics.hpp>

class Edge : public sf::Drawable, public sf::Transformable
{
public:
	Edge(const sf::Vector2f& begin, const sf::Vector2f& end, bool selected);
	~Edge();
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
		override;
private:
	sf::VertexArray m_vertices;
};
