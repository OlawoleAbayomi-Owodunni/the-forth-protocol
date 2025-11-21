#include "AI.h"
#include <algorithm>
#include <cmath>
using namespace sf;

AI::AI()
{
}

Move AI::findBestMove(const vector<vector<Piece*>>& board, vector<Piece>& p2Pieces, 
	vector<Piece>& p1Pieces, int gridSize, bool isPlacementPhase, int depth)
{
	vector<vector<Piece*>> boardCopy = board;
	
	int bestScore = INT_MIN;
	Move bestMove;
	int alpha = INT_MIN;
	int beta = INT_MAX;

	vector<Move> possibleMoves = generateMoves(boardCopy, p2Pieces, gridSize, isPlacementPhase);
	
	m_movesConsidered = possibleMoves.size();

	for (const auto& move : possibleMoves) {
		Piece* piece = &p2Pieces[move.pieceIndex];
		applyMove(boardCopy, piece, move.fromRow, move.fromCol, move.toRow, move.toCol);

		// Check if this move wins immediately
		if (hasWon(boardCopy, false)) {
			bestMove = move;
			bestScore = WINNING_SCORE;
			undoMove(boardCopy, piece, move.fromRow, move.fromCol, move.toRow, move.toCol);
			break;
		}

		int score = minimax(boardCopy, p2Pieces, p1Pieces, gridSize, depth - 1, false, isPlacementPhase, alpha, beta);

		// Undo the move
		undoMove(boardCopy, piece, move.fromRow, move.fromCol, move.toRow, move.toCol);

		if (score > bestScore) {
			bestScore = score;
			bestMove = move;
			alpha = max(alpha, bestScore);
		}
	}

	m_bestScore = bestScore;
	m_selectedMove = bestMove;

	return bestMove;
}

int AI::minimax(vector<vector<Piece*>>& board, vector<Piece>& p2Pieces, 
	vector<Piece>& p1Pieces, int gridSize, int depth, bool isMaximizing, 
	bool isPlacementPhase, int alpha, int beta)
{
	// Terminal conditions
	if (depth == 0) {
		return evaluateBoard(board, isPlacementPhase);
	}

	// Check for win conditions
	if (hasWon(board, false)) {
		return WINNING_SCORE + depth; // Prefer faster wins
	}
	if (hasWon(board, true)) {
		return LOSING_SCORE - depth; // Prefer slower losses
	}

	if (isMaximizing) {
		// AI's turn (Player 2)
		int maxEval = INT_MIN;
		vector<Move> moves = generateMoves(board, p2Pieces, gridSize, isPlacementPhase);

		for (const auto& move : moves) {
			Piece* piece = &p2Pieces[move.pieceIndex];
			applyMove(board, piece, move.fromRow, move.fromCol, move.toRow, move.toCol);

			int eval = minimax(board, p2Pieces, p1Pieces, gridSize, depth - 1, false, isPlacementPhase, alpha, beta);

			undoMove(board, piece, move.fromRow, move.fromCol, move.toRow, move.toCol);

			maxEval = max(maxEval, eval);
			alpha = max(alpha, eval);

			// Alpha-beta pruning
			if (beta <= alpha) {
				break;
			}
		}

		return maxEval;
	} else {
		int minEval = INT_MAX;
		vector<Move> moves = generateMoves(board, p1Pieces, gridSize, isPlacementPhase);

		for (const auto& move : moves) {
			Piece* piece = &p1Pieces[move.pieceIndex];
			applyMove(board, piece, move.fromRow, move.fromCol, move.toRow, move.toCol);

			int eval = minimax(board, p2Pieces, p1Pieces, gridSize, depth - 1, true, isPlacementPhase, alpha, beta);

			undoMove(board, piece, move.fromRow, move.fromCol, move.toRow, move.toCol);

			minEval = min(minEval, eval);
			beta = min(beta, eval);

			// Alpha-beta pruning
			if (beta <= alpha) {
				break;
			}
		}

		return minEval;
	}
}

int AI::evaluateBoard(const vector<vector<Piece*>>& board, bool isPlacementPhase)
{
	int score = 0;
	int gridSize = board.size();

	// Score for AI (Player 2)
	for (int row = 0; row < gridSize; ++row) {
		for (int col = 0; col < gridSize; ++col) {
			if (board[row][col] && !board[row][col]->isPlayer1()) {
				int lineCount = 0;

				// Horizontal
				lineCount = countInLine(board, row, col, 0, 1, false);
				score += lineCount * lineCount * 10; // Quadratic scoring for longer lines

				// Vertical
				lineCount = countInLine(board, row, col, 1, 0, false);
				score += lineCount * lineCount * 10;

				// Diagonal (\)
				lineCount = countInLine(board, row, col, 1, 1, false);
				score += lineCount * lineCount * 10;

				// Diagonal (/)
				lineCount = countInLine(board, row, col, 1, -1, false);
				score += lineCount * lineCount * 10;
			}
		}
	}

	// Subtract score for Player 1
	for (int row = 0; row < gridSize; ++row) {
		for (int col = 0; col < gridSize; ++col) {
			if (board[row][col] && board[row][col]->isPlayer1()) {
				// Check lines of player 1 pieces
				int lineCount = 0;

				// Horizontal
				lineCount = countInLine(board, row, col, 0, 1, true);
				score -= lineCount * lineCount * 10;

				// Vertical
				lineCount = countInLine(board, row, col, 1, 0, true);
				score -= lineCount * lineCount * 10;

				// Diagonal (\)
				lineCount = countInLine(board, row, col, 1, 1, true);
				score -= lineCount * lineCount * 10;

				// Diagonal (/)
				lineCount = countInLine(board, row, col, 1, -1, true);
				score -= lineCount * lineCount * 10;
			}
		}
	}

	return score;
}

int AI::countInLine(const vector<vector<Piece*>>& board, int row, int col, int dRow, int dCol, bool isPlayer1)
{
	int count = 1;
	int gridSize = board.size();

	// Count in positive direction
	int r = row + dRow;
	int c = col + dCol;
	while (r >= 0 && r < gridSize && c >= 0 && c < gridSize && board[r][c] && board[r][c]->isPlayer1() == isPlayer1) {
		count++;
		r += dRow;
		c += dCol;
	}

	// Count in negative direction
	r = row - dRow;
	c = col - dCol;
	while (r >= 0 && r < gridSize && c >= 0 && c < gridSize && board[r][c] && board[r][c]->isPlayer1() == isPlayer1) {
		count++;
		r -= dRow;
		c -= dCol;
	}

	return count;
}

vector<Move> AI::generateMoves(const vector<vector<Piece*>>& board, vector<Piece>& playerPieces, 
	int gridSize, bool isPlacementPhase)
{
	vector<Move> moves;

	if (isPlacementPhase) {
		// During placement, find unplaced pieces and empty cells
		vector<pair<int, int>> emptyCells = getEmptyCells(board, gridSize);

		for (int i = 0; i < playerPieces.size(); ++i) {
			if (playerPieces[i].getGridRow() < 0) {
				for (const auto& cell : emptyCells) {
					moves.push_back(Move(i, -1, -1, cell.first, cell.second));
				}
			}
		}
	} else {
		for (int i = 0; i < playerPieces.size(); ++i) {
			int fromRow = playerPieces[i].getGridRow();
			int fromCol = playerPieces[i].getGridCol();

			if (fromRow >= 0 && fromCol >= 0) {
				auto validMoves = playerPieces[i].getValidMoves(board, gridSize);
				for (const auto& target : validMoves) {
					moves.push_back(Move(i, fromRow, fromCol, target.first, target.second));
				}
			}
		}
	}

	return moves;
}

void AI::applyMove(vector<vector<Piece*>>& board, Piece* piece, int fromRow, int fromCol, int toRow, int toCol)
{
	if (fromRow >= 0 && fromCol >= 0) {
		board[fromRow][fromCol] = nullptr;
	}
	board[toRow][toCol] = piece;
	piece->setGridPosition(toRow, toCol);
}

void AI::undoMove(vector<vector<Piece*>>& board, Piece* piece, int fromRow, int fromCol, int toRow, int toCol)
{
	board[toRow][toCol] = nullptr;
	if (fromRow >= 0 && fromCol >= 0) {
		board[fromRow][fromCol] = piece;
	}
	piece->setGridPosition(fromRow, fromCol);
}

bool AI::hasWon(const vector<vector<Piece*>>& board, bool isPlayer1)
{
	int gridSize = board.size();
	int winLength = 4; // Still need 4 in a row to win
	
	// Check horizontal
	for (int row = 0; row < gridSize; ++row) {
		for (int col = 0; col <= gridSize - winLength; ++col) {
			bool win = true;
			for (int i = 0; i < winLength; ++i) {
				if (!board[row][col + i] || board[row][col + i]->isPlayer1() != isPlayer1) {
					win = false;
					break;
				}
			}
			if (win) return true;
		}
	}

	// Check vertical
	for (int col = 0; col < gridSize; ++col) {
		for (int row = 0; row <= gridSize - winLength; ++row) {
			bool win = true;
			for (int i = 0; i < winLength; ++i) {
				if (!board[row + i][col] || board[row + i][col]->isPlayer1() != isPlayer1) {
					win = false;
					break;
				}
			}
			if (win) return true;
		}
	}

	// Check diagonal (\)
	for (int row = 0; row <= gridSize - winLength; ++row) {
		for (int col = 0; col <= gridSize - winLength; ++col) {
			bool win = true;
			for (int i = 0; i < winLength; ++i) {
				if (!board[row + i][col + i] || board[row + i][col + i]->isPlayer1() != isPlayer1) {
					win = false;
					break;
				}
			}
			if (win) return true;
		}
	}

	// Check diagonal (/)
	for (int row = 0; row <= gridSize - winLength; ++row) {
		for (int col = winLength - 1; col < gridSize; ++col) {
			bool win = true;
			for (int i = 0; i < winLength; ++i) {
				if (!board[row + i][col - i] || board[row + i][col - i]->isPlayer1() != isPlayer1) {
					win = false;
					break;
				}
			}
			if (win) return true;
		}
	}

	return false;
}

vector<pair<int, int>> AI::getEmptyCells(const vector<vector<Piece*>>& board, int gridSize)
{
	vector<pair<int, int>> emptyCells;
	for (int row = 0; row < gridSize; ++row) {
		for (int col = 0; col < gridSize; ++col) {
			if (board[row][col] == nullptr) {
				emptyCells.push_back(make_pair(row, col));
			}
		}
	}
	return emptyCells;
}
