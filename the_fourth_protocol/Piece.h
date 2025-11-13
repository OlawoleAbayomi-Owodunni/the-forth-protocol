#pragma once
#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;
using namespace std;

class Piece
{
	public:
	enum class Type {
		Frog,
		Snake,
		Donkey
	};

	Piece(Type type, const sf::Texture& texture, float size, Vector2f startPos, bool p1)
		: m_type(type)
	{
		m_rect.setTexture(&texture);
		m_rect.setSize(Vector2f(size, size));
		if (p1) m_rect.setFillColor(Color::Red);
		else m_rect.setFillColor(Color::Blue);
		m_rect.setPosition(startPos);
	}

	Type getType() const { return m_type; }

	void draw(RenderWindow& window) const
	{
		window.draw(m_rect);
	}

private:
	Type m_type;
	RectangleShape m_rect;
};

