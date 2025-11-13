#include "Piece.h"

Piece::Piece(Type type, const sf::Texture& texture, float size, Vector2f startPos, bool p1)
	: m_type(type)
{
	m_rect.setTexture(&texture);
	m_rect.setSize(Vector2f(size, size));
	if (p1) m_rect.setFillColor(Color::Red);
	else m_rect.setFillColor(Color::Blue);
	m_rect.setPosition(startPos);
}

bool Piece::contains(Vector2f point) const {
	return m_rect.getGlobalBounds().contains(point);
}

void Piece::setSelected(bool selected) {
	m_isSelected = selected;
	if (selected) {
		m_rect.setOutlineThickness(3.0f);
		m_rect.setOutlineColor(Color::Yellow);
	}
	else {
		m_rect.setOutlineThickness(0.0f);
	}
}

void Piece::saveOriginalPosition() {
	m_originalPosition = m_rect.getPosition();
}

void Piece::restoreOriginalPosition() {
	m_rect.setPosition(m_originalPosition);
}
