/**
 * @file Piece.h
 * @brief Game piece class for The Fourth Protocol
 * @author OA-O  
 * @date November 2025
 * @version 1.0
 *
 * This file contains the Piece class which represents individual game pieces
 * with different types (Frog, Snake, Donkey) and their movement rules.
 */

#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

using namespace sf;
using namespace std;

/// @brief Represents a game piece that can be placed and moved on the board
/// 
/// This class manages individual pieces in The Fourth Protocol game, including
/// their type, position, movement validation, and rendering properties.
class Piece
{
	public:
	/// @brief Enumeration of different piece types with unique movement rules
	enum class Type {
		Frog,		///< Can jump over other pieces
		Snake,		///< Moves in straight lines only
		Donkey,		///< Basic movement piece
		Antelope,	///< Moves L-shape like a knight in chess
		Lion		///< Moves ONLY to 2 spaces in any direction
	};

    /// @brief Constructor to create a new piece
    /// @param type The type of piece (Frog, Snake, or Donkey)
    /// @param texture Reference to the texture to use for rendering
    /// @param size Size of the piece in pixels
    /// @param startPos Initial position of the piece
    /// @param p1 True if this piece belongs to Player 1, false for Player 2
    Piece(Type type, const sf::Texture& texture, float size, Vector2f startPos, bool p1);

	/// @brief Gets the type of this piece
	/// @return The piece type (Frog, Snake, or Donkey)
	Type getType() const { return m_type; };
	
	/// @brief Checks if this piece belongs to Player 1
	/// @return True if Player 1's piece, false if Player 2's piece
	bool isPlayer1() const { return m_isPlayer1; };
	
	/// @brief Gets the current grid row position
	/// @return Current row on the game board (-1 if not placed)
	int getGridRow() const { return m_gridRow; };
	
	/// @brief Gets the current grid column position  
	/// @return Current column on the game board (-1 if not placed)
	int getGridCol() const { return m_gridCol; };

	/// @brief Draws the piece to the render window
	/// @param window Reference to the SFML render window
	void draw(RenderWindow& window) const { window.draw(m_rect); };
	
	/// @brief Checks if a point is contained within this piece's bounds
	/// @param point Point to test for containment
	/// @return True if point is inside the piece, false otherwise
	bool contains(Vector2f point) const;

	/// @brief Sets the visual position of the piece
	/// @param pos New position for the piece
	void setPosition(Vector2f pos) { m_rect.setPosition(pos); };
	
	/// @brief Gets the current visual position of the piece
	/// @return Current position of the piece
	Vector2f getPosition() const { return m_rect.getPosition(); };
    
	/// @brief Gets the bounding rectangle of the piece
	/// @return FloatRect representing the piece's bounds
	FloatRect getBounds() const { return m_rect.getGlobalBounds(); };

	/// @brief Sets the visual selection state of the piece
	/// @param selected True to show as selected, false for normal appearance
	void setSelected(bool selected);

	/// @brief Saves the current position for potential restoration
	void saveOriginalPosition();
	
	/// @brief Restores the piece to its previously saved position
	void restoreOriginalPosition();

	/// @brief Sets the logical grid position of the piece
	/// @param row Grid row position
	/// @param col Grid column position
	void setGridPosition(int row, int col) { m_gridRow = row; m_gridCol = col; };

	/// @brief Check if a move to the target position is valid for this piece
	/// @param targetRow Target grid row
	/// @param targetCol Target grid column
	/// @param board Current board state with piece positions
	/// @param gridSize Size of the game grid
	/// @return True if move is valid according to piece movement rules, false otherwise
	bool isValidMove(int targetRow, int targetCol, const vector<vector<Piece*>>& board, int gridSize);

	/// @brief Get all valid moves available for this piece from its current position
	/// @param board Current board state with piece positions
	/// @param gridSize Size of the game grid
	/// @return Vector of coordinate pairs representing all valid target positions
	vector<pair<int, int>> getValidMoves(const vector<vector<Piece*>>& board, int gridSize);

private:
	/// @brief The type of this piece (affects movement rules)
	Type m_type;
	/// @brief SFML rectangle shape used for rendering and collision detection
	RectangleShape m_rect;
	/// @brief Flag indicating if this piece is currently selected
	bool m_isSelected = false;
	/// @brief Saved position for drag-and-drop restoration
	Vector2f m_originalPosition;
	/// @brief Flag indicating which player owns this piece
	bool m_isPlayer1;
	/// @brief Current logical grid row position (-1 if not on board)
	int m_gridRow = -1;
	/// @brief Current logical grid column position (-1 if not on board)
	int m_gridCol = -1;

	/// @brief Helper function to validate movement to a target position
	/// @param targetRow Target grid row
	/// @param targetCol Target grid column
	/// @param board Current board state
	/// @param gridSize Size of the game grid
	/// @return True if move is allowed, false otherwise
	bool canMoveTo(int targetRow, int targetCol, const vector<vector<Piece*>>& board, int gridSize);
	
	/// @brief Helper function to check if piece can jump to target (Frog-specific)
	/// @param targetRow Target grid row
	/// @param targetCol Target grid column
	/// @param board Current board state
	/// @param gridSize Size of the game grid
	/// @return True if jump is valid, false otherwise
	bool canJump(int targetRow, int targetCol, const vector<vector<Piece*>>& board, int gridSize);
	
	/// @brief Checks if two positions form a straight line (horizontal, vertical, diagonal)
	/// @param fromRow Starting grid row
	/// @param fromCol Starting grid column
	/// @param toRow Target grid row
	/// @param toCol Target grid column
	/// @return True if positions form a line, false otherwise
	bool isInLine(int fromRow, int fromCol, int toRow, int toCol);
	
	/// @brief Checks if the path between two positions is clear of other pieces
	/// @param fromRow Starting grid row
	/// @param fromCol Starting grid column
	/// @param toRow Target grid row
	/// @param toCol Target grid column
	/// @param board Current board state
	/// @return True if path is clear, false if blocked
	bool isPathClear(int fromRow, int fromCol, int toRow, int toCol, const vector<vector<Piece*>>& board);
};

