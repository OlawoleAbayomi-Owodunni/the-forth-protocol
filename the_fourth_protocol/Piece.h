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

    Piece(Type type, const sf::Texture& texture, float size, Vector2f startPos, bool p1);

	Type getType() const { return m_type; };

	void draw(RenderWindow& window) const { window.draw(m_rect); };
	bool contains(Vector2f point) const;

	void setPosition(Vector2f pos) { m_rect.setPosition(pos); };
	Vector2f getPosition() const { return m_rect.getPosition(); };
    
	FloatRect getBounds() const { return m_rect.getGlobalBounds(); };

	void setSelected(bool selected);

	void saveOriginalPosition();
	void restoreOriginalPosition();

private:
	Type m_type;
	RectangleShape m_rect;
	bool m_isSelected = false;
	Vector2f m_originalPosition;
};

