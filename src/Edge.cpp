#include "Edge.h"

Edge::Edge(const sf::Vector2f& begin, const sf::Vector2f& end, bool selected) :
	m_vertices(sf::TrianglesFan, 7)
{
	sf::Vector2f dir = end - begin;
	float mag = 1.0f / std::sqrt(dir.x * dir.x + dir.y * dir.y);
	dir.x *= mag;
	dir.y *= mag;

	sf::Vector2f head = end - 12.0f * dir;

	m_vertices[0].position = sf::Vector2f(end.x, end.y);

	m_vertices[1].position = sf::Vector2f(
		head.x - 6.0f * dir.y,
		head.y - 6.0f * (-dir.x));
	m_vertices[2].position = sf::Vector2f(
		head.x - 2.0f * dir.y,
		head.y - 2.0f * (-dir.x));
	m_vertices[3].position = sf::Vector2f(
		begin.x - 2.0f * dir.y,
		begin.y - 2.0f * (-dir.x));
	m_vertices[4].position = sf::Vector2f(
		begin.x + 2.0f * dir.y,
		begin.y + 2.0f * (-dir.x));
	m_vertices[5].position = sf::Vector2f(
		head.x + 2.0f * dir.y,
		head.y + 2.0f * (-dir.x));
	m_vertices[6].position = sf::Vector2f(
		head.x + 6.0f * dir.y,
		head.y + 6.0f * (-dir.x));

	sf::Color color(0, 0, 0);
	if (selected)
	{
		color = sf::Color(255, 0, 0);
	}

	for (unsigned i = 0; i < 7; ++i)
	{
		m_vertices[i].color = color;
	}
}

Edge::~Edge()
{

}

void Edge::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(m_vertices, states);
}
