/**
 * @file AI.h
 * @brief Artificial Intelligence player for The Fourth Protocol
 * @author RCH and OA-O
 * @date November 2025
 * @version 1.0
 *
 * This file contains the AI class that implements a computer opponent
 * using the minimax algorithm with alpha-beta pruning for move selection.
 */

#pragma once
#include <vector>
#include <tuple>
#include <limits>
#include "Piece.h"

using namespace std;

/// @brief AI evaluation strategy types
enum class Strategy {
	Balanced,        ///< Balanced approach to all directions and positions
	FavorDiagonal,   ///< Prioritize diagonal alignments
	FavorCenter,     ///< Prioritize center control
	FavorEdges,      ///< Prioritize edge positions
	Aggressive,      ///< Focus on offense over defense
	Defensive        ///< Focus on blocking opponent
};

/// @brief Represents a move in the game (piece index and target position)
///
/// This structure encapsulates all information needed to represent a single
/// move in the game, including the piece to move and its source/target positions.
struct Move {
	/// @brief Index of the piece in the player's piece collection
	int pieceIndex;
	/// @brief Source grid row position
	int fromRow;
	/// @brief Source grid column position
	int fromCol;
	/// @brief Target grid row position
	int toRow;
	/// @brief Target grid column position
	int toCol;

	/// @brief Default constructor creating an invalid move
	Move() : pieceIndex(-1), fromRow(-1), fromCol(-1), toRow(-1), toCol(-1) {}
	
	/// @brief Parameterized constructor to create a specific move
	/// @param pi Piece index in the collection
	/// @param fr Source row
	/// @param fc Source column
	/// @param tr Target row
	/// @param tc Target column
	Move(int pi, int fr, int fc, int tr, int tc) 
		: pieceIndex(pi), fromRow(fr), fromCol(fc), toRow(tr), toCol(tc) {}
};

/// @brief AI player implementation using minimax algorithm with alpha-beta pruning
///
/// This class provides artificial intelligence for The Fourth Protocol game,
/// using game tree search to find optimal moves for the computer player.
class AI
{
public:
	/// @brief Default constructor for AI player
	AI();

	/// @brief Find the best move for Player 2 (AI) using minimax algorithm with alpha-beta pruning
	/// @param board Current state of the game board
	/// @param p2Pieces Reference to Player 2's pieces (AI's pieces)
	/// @param p1Pieces Reference to Player 1's pieces (human player's pieces)
	/// @param gridSize Size of the game grid
	/// @param isPlacementPhase True if in placement phase, false if in movement phase
	/// @param depth Maximum search depth for the minimax algorithm
	/// @param useRandomPlacement If true, selects random placement instead of strategic (for AI vs AI variety)
	/// @param lastMove The last move made (to avoid immediately undoing it)
	/// @param strategy The evaluation strategy to use (default: Balanced)
	/// @return Move object representing the best move found
	Move findBestMove(const vector<vector<Piece*>>& board, vector<Piece>& p2Pieces, 
		vector<Piece>& p1Pieces, int gridSize, bool isPlacementPhase, int depth = 3, 
		bool useRandomPlacement = false, const Move& lastMove = Move(), Strategy strategy = Strategy::Balanced);

	/// @brief Get the number of moves considered in the last decision
	/// @return Number of possible moves evaluated
	int getMovesConsidered() const { return m_movesConsidered; }
	
	/// @brief Get the score of the best move found
	/// @return Evaluation score of the selected move
	int getBestScore() const { return m_bestScore; }
	
	/// @brief Get the best move from the last search
	/// @return The Move object that was selected
	Move getSelectedMove() const { return m_selectedMove; }

private:
	/// @brief Maximum search depth for the minimax algorithm
	static const int MAX_DEPTH = 3;
	/// @brief Score value representing a winning position
	static const int WINNING_SCORE = 10000;
	/// @brief Score value representing a losing position
	static const int LOSING_SCORE = -10000;

	/// @brief Number of moves considered in last search
	int m_movesConsidered = 0;
	/// @brief Score of the best move found
	int m_bestScore = 0;
	/// @brief The selected move from last search
	Move m_selectedMove;
	/// @brief Current evaluation strategy
	Strategy m_strategy = Strategy::Balanced;

	/// @brief Minimax algorithm implementation with alpha-beta pruning
	/// @param board Current board state (modified during search)
	/// @param p2Pieces Reference to Player 2's pieces
	/// @param p1Pieces Reference to Player 1's pieces
	/// @param gridSize Size of the game grid
	/// @param depth Current search depth remaining
	/// @param isMaximizing True if maximizing player's turn (AI), false for minimizing (human)
	/// @param isPlacementPhase True if in placement phase, false if in movement phase
	/// @param alpha Alpha value for alpha-beta pruning
	/// @param beta Beta value for alpha-beta pruning
	/// @return Evaluated score for the current position
	int minimax(vector<vector<Piece*>>& board, vector<Piece>& p2Pieces, 
		vector<Piece>& p1Pieces, int gridSize, int depth, bool isMaximizing, 
		bool isPlacementPhase, int alpha, int beta);

	/// @brief Evaluate the current board state and return a score
	/// @param board Current board state to evaluate
	/// @param isPlacementPhase True if in placement phase, false if in movement phase
	/// @return Numerical score representing board favorability (positive = good for AI)
	int evaluateBoard(const vector<vector<Piece*>>& board, bool isPlacementPhase);

	/// @brief Count consecutive pieces in a line for scoring purposes
	/// @param board Current board state
	/// @param row Starting row position
	/// @param col Starting column position
	/// @param dRow Row direction vector (-1, 0, or 1)
	/// @param dCol Column direction vector (-1, 0, or 1)
	/// @param isPlayer1 True to count Player 1's pieces, false for Player 2's pieces
	/// @return Number of consecutive pieces found in the specified direction
	int countInLine(const vector<vector<Piece*>>& board, int row, int col, int dRow, int dCol, bool isPlayer1);

	/// @brief Generate all possible legal moves for a player
	/// @param board Current board state
	/// @param playerPieces Reference to the player's pieces
	/// @param gridSize Size of the game grid
	/// @param isPlacementPhase True if in placement phase, false if in movement phase
	/// @return Vector of all possible Move objects for the player
	vector<Move> generateMoves(const vector<vector<Piece*>>& board, vector<Piece>& playerPieces, 
		int gridSize, bool isPlacementPhase);

	/// @brief Apply a move to the board state (used during search)
	/// @param board Board state to modify
	/// @param piece Pointer to the piece to move
	/// @param fromRow Source row position
	/// @param fromCol Source column position
	/// @param toRow Target row position
	/// @param toCol Target column position
	void applyMove(vector<vector<Piece*>>& board, Piece* piece, int fromRow, int fromCol, int toRow, int toCol);

	/// @brief Undo a move on the board state (used during search backtracking)
	/// @param board Board state to modify
	/// @param piece Pointer to the piece to move back
	/// @param fromRow Original row position
	/// @param fromCol Original column position
	/// @param toRow Current row position
	/// @param toCol Current column position
	void undoMove(vector<vector<Piece*>>& board, Piece* piece, int fromRow, int fromCol, int toRow, int toCol);

	/// @brief Check if a player has achieved a win condition (4 in a row)
	/// @param board Current board state to check
	/// @param isPlayer1 True to check Player 1's win condition, false for Player 2
	/// @return True if the player has won, false otherwise
	bool hasWon(const vector<vector<Piece*>>& board, bool isPlayer1);

	/// @brief Find all empty cells on the board
	/// @param board Current board state
	/// @param gridSize Size of the game grid
	/// @return Vector of coordinate pairs representing empty positions
	vector<pair<int, int>> getEmptyCells(const vector<vector<Piece*>>& board, int gridSize);

	int scoreCloserToCenter(int row, int col, int gridSize);
};
