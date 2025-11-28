#include "Piece.h"
#include <cmath>
#include <algorithm>

Piece::Piece(Type type, const sf::Texture& texture, float size, Vector2f startPos, bool p1)
	: m_type(type), m_isPlayer1(p1)
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

bool Piece::isInLine(int fromRow, int fromCol, int toRow, int toCol) {
	if (fromRow == toRow || fromCol == toCol) return true; // horizontal or vertical
	if (abs(fromRow - toRow) == abs(fromCol - toCol)) return true; // diagonal
	return false;
}

bool Piece::isPathClear(int fromRow, int fromCol, int toRow, int toCol, const vector<vector<Piece*>>& board) {
	int rowDir = 0, colDir = 0;
	
	if (toRow > fromRow) rowDir = 1;
	else if (toRow < fromRow) rowDir = -1;
	
	if (toCol > fromCol) colDir = 1;
	else if (toCol < fromCol) colDir = -1;
	
	int curRow = fromRow + rowDir;
	int curCol = fromCol + colDir;
	
	while (curRow != toRow || curCol != toCol) {
		if (board[curRow][curCol] != nullptr) return false;
		curRow += rowDir;
		curCol += colDir;
	}
	
	return true;
}

bool Piece::canMoveTo(int targetRow, int targetCol, const vector<vector<Piece*>>& board, int gridSize) {
	if (targetRow < 0 || targetRow >= gridSize || targetCol < 0 || targetCol >= gridSize) return false;
	
	// Target must be empty
	if (board[targetRow][targetCol] != nullptr) return false;
	
	if (m_type == Type::Antelope) {
		// Antelope: L-shape move like a knight in chess
		int rowDist = abs(targetRow - m_gridRow);
		int colDist = abs(targetCol - m_gridCol);

		if ((rowDist == 2 && colDist == 1) || (rowDist == 1 && colDist == 2)) {
			return true;
		}
	}
	else if (m_type == Type::Lion) {
		// Lion: Moves ONLY to 2 spaces in any direction
		int rowDist = abs(targetRow - m_gridRow);
		int colDist = abs(targetCol - m_gridCol);

		if ((rowDist == 2 && colDist == 0) || // vertical
			(rowDist == 0 && colDist == 2) || // horizontal
			(rowDist == 2 && colDist == 2) ||	// diagonal
			(rowDist == 2 && colDist == 1) || (rowDist == 1 && colDist == 2) // everything else
			) {
			return true;
		}

		return false;
	}

	if (!isInLine(m_gridRow, m_gridCol, targetRow, targetCol)) return false;
	
	if (m_type == Type::Donkey) {
		// Donkey: only one space in any direction, NOT diagonally
		int rowDist = abs(targetRow - m_gridRow);
		int colDist = abs(targetCol - m_gridCol);
		
		// Must move exactly 1 space in one direction (not diagonal)
		if ((rowDist == 1 && colDist == 0) || (rowDist == 0 && colDist == 1)) {
			return true;
		}
		return false;
	}
	else if (m_type == Type::Snake || m_type == Type::Frog) {
		// Snake and Frog: only one space in any direction INCLUDING diagonally
		int rowDist = abs(targetRow - m_gridRow);
		int colDist = abs(targetCol - m_gridCol);
		
		// Must move exactly 1 space (can be diagonal)
		if (rowDist <= 1 && colDist <= 1 && (rowDist + colDist > 0)) {
			return true;
		}
		return false;
	}
	return false;
}

bool Piece::canJump(int targetRow, int targetCol, const vector<vector<Piece*>>& board, int gridSize) {
	if (m_type != Type::Frog) return false;
	
	// Target must be within bounds and empty
	if (targetRow < 0 || targetRow >= gridSize || targetCol < 0 || targetCol >= gridSize) return false;
	if (board[targetRow][targetCol] != nullptr) return false;
	
	if (!isInLine(m_gridRow, m_gridCol, targetRow, targetCol)) return false;
	
	int rowDir = 0, colDir = 0;
	if (targetRow > m_gridRow) rowDir = 1;
	else if (targetRow < m_gridRow) rowDir = -1;
	if (targetCol > m_gridCol) colDir = 1;
	else if (targetCol < m_gridCol) colDir = -1;
	
	int curRow = m_gridRow + rowDir;
	int curCol = m_gridCol + colDir;
	
	int lastPieceRow = -1;
	int lastPieceCol = -1;
	bool foundAnyPiece = false;
	
	while (curRow != targetRow || curCol != targetCol) {
		if (board[curRow][curCol] != nullptr) {
			lastPieceRow = curRow;
			lastPieceCol = curCol;
			foundAnyPiece = true;
		} else if (foundAnyPiece) {
			return (curRow == targetRow && curCol == targetCol);
		}
		curRow += rowDir;
		curCol += colDir;
	}
	
	if (foundAnyPiece) {
		return (targetRow == lastPieceRow + rowDir && targetCol == lastPieceCol + colDir);
	}
	
	return false;
}

bool Piece::isValidMove(int targetRow, int targetCol, const vector<vector<Piece*>>& board, int gridSize) {
	if (canMoveTo(targetRow, targetCol, board, gridSize)) {
		return true;
	}
	
	if (canJump(targetRow, targetCol, board, gridSize)) {
		return true;
	}
	
	return false;
}

vector<pair<int, int>> Piece::getValidMoves(const vector<vector<Piece*>>& board, int gridSize) {
	vector<pair<int, int>> validMoves;
	
	// Check all cells on the board
	for (int row = 0; row < gridSize; ++row) {
		for (int col = 0; col < gridSize; ++col) {
			if (row == m_gridRow && col == m_gridCol) continue; // Skip current position
			
			if (isValidMove(row, col, board, gridSize)) {
				validMoves.push_back(make_pair(row, col));
			}
		}
	}
	
	return validMoves;
}
