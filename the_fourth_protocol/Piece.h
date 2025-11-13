#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

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
	bool isPlayer1() const { return m_isPlayer1; };
	int getGridRow() const { return m_gridRow; };
	int getGridCol() const { return m_gridCol; };

	void draw(RenderWindow& window) const { window.draw(m_rect); };
	bool contains(Vector2f point) const;

	void setPosition(Vector2f pos) { m_rect.setPosition(pos); };
	Vector2f getPosition() const { return m_rect.getPosition(); };
    
	FloatRect getBounds() const { return m_rect.getGlobalBounds(); };

	void setSelected(bool selected);

	void saveOriginalPosition();
	void restoreOriginalPosition();

	void setGridPosition(int row, int col) { m_gridRow = row; m_gridCol = col; };

	// Check if a move to (targetRow, targetCol) is valid for this piece
	bool isValidMove(int targetRow, int targetCol, const vector<vector<Piece*>>& board, int gridSize);

	// Get all valid moves for this piece
	vector<pair<int, int>> getValidMoves(const vector<vector<Piece*>>& board, int gridSize);

private:
	Type m_type;
	RectangleShape m_rect;
	bool m_isSelected = false;
	Vector2f m_originalPosition;
	bool m_isPlayer1;
	int m_gridRow = -1;
	int m_gridCol = -1;

	// Helper for movement validation
	bool canMoveTo(int targetRow, int targetCol, const vector<vector<Piece*>>& board, int gridSize);
	bool canJump(int targetRow, int targetCol, const vector<vector<Piece*>>& board, int gridSize);
	bool isInLine(int fromRow, int fromCol, int toRow, int toCol);
	bool isPathClear(int fromRow, int fromCol, int toRow, int toCol, const vector<vector<Piece*>>& board);
};

