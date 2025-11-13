#pragma once
#include <vector>
#include <tuple>
#include <limits>
#include "Piece.h"

using namespace std;

// Represents a move in the game (piece index and target position)
struct Move {
	int pieceIndex;
	int fromRow;
	int fromCol;
	int toRow;
	int toCol;

	Move() : pieceIndex(-1), fromRow(-1), fromCol(-1), toRow(-1), toCol(-1) {}
	Move(int pi, int fr, int fc, int tr, int tc) 
		: pieceIndex(pi), fromRow(fr), fromCol(fc), toRow(tr), toCol(tc) {}
};

class AI
{
public:
	AI();

	/// <summary>
	/// Find the best move for Player 2 (AI) using minimax algorithm with alpha-beta pruning
	/// </summary>
	Move findBestMove(const vector<vector<Piece*>>& board, vector<Piece>& p2Pieces, 
		vector<Piece>& p1Pieces, int gridSize, bool isPlacementPhase, int depth = 3);

private:
	static const int MAX_DEPTH = 3;
	static const int WINNING_SCORE = 10000;
	static const int LOSING_SCORE = -10000;

	/// <summary>
	/// Minimax algorithm with alpha-beta pruning
	/// </summary>
	int minimax(vector<vector<Piece*>>& board, vector<Piece>& p2Pieces, 
		vector<Piece>& p1Pieces, int gridSize, int depth, bool isMaximizing, 
		bool isPlacementPhase, int alpha, int beta);

	/// <summary>
	/// Evaluate the current board state
	/// </summary>
	int evaluateBoard(const vector<vector<Piece*>>& board, bool isPlacementPhase);

	/// <summary>
	/// Count pieces in a line for scoring
	/// </summary>
	int countInLine(const vector<vector<Piece*>>& board, int row, int col, int dRow, int dCol, bool isPlayer1);

	/// <summary>
	/// Generate all possible moves for a player
	/// </summary>
	vector<Move> generateMoves(const vector<vector<Piece*>>& board, vector<Piece>& playerPieces, 
		int gridSize, bool isPlacementPhase);

	/// <summary>
	/// Apply a move to the board
	/// </summary>
	void applyMove(vector<vector<Piece*>>& board, Piece* piece, int fromRow, int fromCol, int toRow, int toCol);

	/// <summary>
	/// Undo a move on the board
	/// </summary>
	void undoMove(vector<vector<Piece*>>& board, Piece* piece, int fromRow, int fromCol, int toRow, int toCol);

	/// <summary>
	/// Check if a player has won
	/// </summary>
	bool hasWon(const vector<vector<Piece*>>& board, bool isPlayer1);

	/// <summary>
	/// Find empty cells on the board
	/// </summary>
	vector<pair<int, int>> getEmptyCells(const vector<vector<Piece*>>& board, int gridSize);
};
