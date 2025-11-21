/**
 * @file Game.h
 * @brief Main game class for The Fourth Protocol
 * @author OA-O
 * @date November 2025
 * @version 1.0
 *
 * This file contains the Game class which manages the main game loop,
 * player interactions, AI opponent, and game state for The Fourth Protocol.
 */

#pragma once

// If VS Debug build is enabled, then any block of code enclosed within 
//  the preprocessor directive #ifdef TEST_FPS is compiled and executed.
#ifdef _DEBUG
#define TEST_FPS
#endif // _DEBUG

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Piece.h"
#include "AI.h"

using namespace std;
using namespace sf;

/// <summary>
/// @author OA-O
/// @date November 2025
/// @version 1.0
/// 
/// </summary>

/// <summary>
/// @brief Main class for The Fourth Protocol game.
/// 
/// This implements a 5x5 grid-based game where players place pieces
/// and move them to get 4 in a row.
/// </summary>

/// <summary>
/// @brief Screen dimensions structure for the game window.
/// 
/// Contains static constants defining the default window size for the game.
/// </summary>
struct ScreenSize
{
public:
	/// @brief Game window width in pixels
	static const int s_width{ 1920 };

	/// @brief Game window height in pixels
	static const int s_height{ 1080 };
};

class Game
{
public:
	/// @brief Enumeration representing different phases of the game
	enum class GamePhase {
		Placement,		///< Players placing pieces on the board
		Movement,		///< Players moving pieces already on the board  
		GameOver		///< Game has finished with a winner
	};

	/// <summary>
	/// @brief Default constructor that initialises the SFML window, 
	///   and sets vertical sync enabled. 
	/// </summary>
	Game();

	/// <summary>
	/// @brief the main game loop.
	/// 
	/// A complete loop involves processing SFML events, updating and drawing all game objects.
	/// The actual elapsed time for a single game loop is calculated. If this value is 
	///  greater than the target time for one loop (1 / 60), then (and only then) is an update 
	///  operation performed.
	/// The target is at least one update and one render cycle per game loop, but typically 
	///  more render than update operations will be performed as we expect our game loop to
	///  complete in less than the target time.
	/// </summary>
	void run();

#pragma region protected

protected:
	/// <summary>
	/// @brief Once-off game initialisation code
	/// </summary>	
	void init();
	/// <summary>
	/// @brief Placeholder to perform updates to all game objects.
	/// </summary>
	/// <param name="dt">Update delta time in seconds</param>
	void update(double dt);

	/// <summary>
	/// @brief Draws the background and foreground game objects in the SFML window.
	/// The render window is always cleared to black before anything is drawn.
	/// </summary>
	void render();

	/// <summary>
	/// @brief Checks for events.
	/// Allows window to function and exit. 
	/// Events are passed on to the Game::processGameEvents() method.
	/// </summary>	
	void processEvents();

	/// <summary>
	/// @brief Handles all user input.
	/// </summary>
	/// <param name="event">system event</param>
	void processGameEvents(const sf::Event&);

	/// @brief Font used for all text rendering in the game
	sf::Font m_arialFont{ "ASSETS/FONTS/ariblk.ttf" };
	/// @brief Main SFML render window for the game
	sf::RenderWindow m_window;

#ifdef TEST_FPS
	/// @brief Text object displaying updates per second (debug only)
	sf::Text x_updateFPS{ m_arialFont };
	/// @brief Text object displaying draw calls per second (debug only)
	sf::Text x_drawFPS{  m_arialFont };
	/// @brief Timer to track when a second has passed (debug only)
	sf::Time x_secondTime{ sf::Time::Zero };
	/// @brief Counter for updates per second (debug only)
	int x_updateFrameCount{ 0 };
	/// @brief Counter for draws per second (debug only)
	int x_drawFrameCount{ 0 };
#endif // TEST_FPS
#pragma endregion

private:
	/// @brief Sets up a grid of RectangleShapes for the game board
	/// @param grid Vector of RectangleShape objects to populate
	/// @param row Starting row position for the grid
	/// @param col Starting column position for the grid  
	/// @param numCols Number of columns in the grid
	/// @param cellSizeXY Size of each grid cell in pixels
	/// @param x0 X-coordinate offset for grid positioning
	/// @param y0 Y-coordinate offset for grid positioning
	/// @param outlineColour Color for the grid cell outlines
	void setupGrid(vector<RectangleShape>& grid, int row, int col, int numCols, const float cellSizeXY, const float x0, const float y0, sf::Color outlineColour);
	
	/// @brief Sets up pieces for a player at game start
	/// @param pieces Vector of Piece objects to initialize
	/// @param row Starting row for piece placement
	/// @param cellSizeXY Size of each cell for piece sizing
	/// @param startPos Starting position for piece placement
	/// @param isP1 True if setting up Player 1 pieces, false for Player 2
	void setupPieces(vector<Piece>& pieces, int row, const float cellSizeXY, Vector2f startPos, bool isP1);

	/// @brief Snaps a piece position to the nearest valid grid location
	/// @param mousePos Current mouse position to snap to grid
	void snapToGrid(Vector2f mousePos);

	// Game logic methods
	/// @brief Checks if a piece placement at the given position is valid
	/// @param row Grid row to check
	/// @param col Grid column to check
	/// @return True if placement is valid, false otherwise
	bool isValidPlacement(int row, int col);
	
	/// @brief Places a piece at the specified grid position
	/// @param piece Pointer to the piece to place
	/// @param row Target grid row
	/// @param col Target grid column
	/// @return True if placement was successful, false otherwise
	bool placePiece(Piece* piece, int row, int col);
	
	/// @brief Moves a piece from one grid position to another
	/// @param piece Pointer to the piece to move
	/// @param fromRow Source grid row
	/// @param fromCol Source grid column
	/// @param toRow Target grid row
	/// @param toCol Target grid column
	/// @return True if move was successful, false otherwise
	bool movePiece(Piece* piece, int fromRow, int fromCol, int toRow, int toCol);
	
	/// @brief Ends the current player's turn and switches to the other player
	void endTurn();
	
	/// @brief Checks if the specified player has achieved a win condition (4 in a row)
	/// @param isPlayer1 True to check Player 1's win condition, false for Player 2
	/// @return True if player has won, false otherwise
	bool checkWinCondition(bool isPlayer1);
	
	/// @brief Gets a pointer to the piece at the specified grid position
	/// @param row Grid row to check
	/// @param col Grid column to check
	/// @return Pointer to piece at position, or nullptr if empty
	Piece* getPieceAtGridPosition(int row, int col);
	
	/// @brief Updates the internal board state representation
	void updateBoard();

	// AI methods
	/// @brief Executes an AI move during the AI player's turn
	void executeAIMove();
	
	/// @brief Applies the given move to the game state
	/// @param move The Move object containing move details to apply
	void applyAIMove(const Move& move);

	// Board state
	/// @brief 2D vector representing the game board state with piece pointers
	vector<vector<Piece*>> m_board;

	/// @brief Grid rectangles for the main game board visualization
	vector<RectangleShape> m_grid;
	/// @brief Number of rows in the game grid
	int m_gridRows{ 8 };
	/// @brief Number of columns in the game grid
	int m_gridCols{ 8 };

	/// @brief Grid rectangles for Player 1's piece area
	vector<RectangleShape> m_p1Grid;
	/// @brief Grid rectangles for Player 2's piece area
	vector<RectangleShape> m_p2Grid;

	/// @brief Texture for frog pieces
	Texture m_frogTexture{ "ASSETS/IMAGES/frog.png" };
	/// @brief Texture for snake pieces
	Texture m_snakeTexture{ "ASSETS/IMAGES/snake.png" };
	/// @brief Texture for donkey pieces
	Texture m_donkeyTexture{ "ASSETS/IMAGES/donkey.png" };
	/// @brief Texture for fox pieces
	Texture m_foxTexture{ "ASSETS/IMAGES/fox.png" };
	/// @brief Texture for owl pieces
	Texture m_owlTexture{ "ASSETS/IMAGES/owl.png" };
	/// @brief Texture for lion pieces
	Texture m_lionTexture{ "ASSETS/IMAGES/lion.png" };

	/// @brief Collection of Player 1's pieces
	vector<Piece> m_p1Pieces;
	/// @brief Collection of Player 2's pieces
	vector<Piece> m_p2Pieces;

	/// @brief Pointer to the currently selected piece for dragging
	Piece* m_selectedPiece = nullptr;
	/// @brief Offset for smooth piece dragging
	Vector2f m_dragOffset;
	/// @brief Flag indicating if a piece is currently being dragged
	bool m_isDragging = false;

	// Game state
	/// @brief Current phase of the game (placement, movement, or game over)
	GamePhase m_gamePhase = GamePhase::Placement;
	/// @brief Flag indicating if it's currently Player 1's turn
	bool m_isPlayer1Turn = true;  // Player 1 starts
	/// @brief Number of pieces Player 1 has placed on the board
	int m_p1PiecesPlaced = 0;
	/// @brief Number of pieces Player 2 has placed on the board
	int m_p2PiecesPlaced = 0;
	/// @brief Pointer to the winning piece (nullptr if no winner yet)
	Piece* m_winner = nullptr;

	/// @brief Text object displaying current game status
	sf::Text m_statusText{ m_arialFont };
	/// @brief Text object displaying game instructions
	sf::Text m_instructionText{ m_arialFont };

	// AI Analyzer Display
	/// @brief Background panel for AI analyzer
	sf::RectangleShape m_aiAnalyzerPanel;
	/// @brief Title text for AI analyzer
	sf::Text m_aiAnalyzerTitle{ m_arialFont };
	/// @brief Text showing number of moves considered
	sf::Text m_aiMovesConsideredText{ m_arialFont };
	/// @brief Text showing the best move selected
	sf::Text m_aiBestMoveText{ m_arialFont };
	/// @brief Text showing the evaluation score
	sf::Text m_aiScoreText{ m_arialFont };
	/// @brief Text showing search depth
	sf::Text m_aiDepthText{ m_arialFont };
	/// @brief Text showing time taken to calculate
	sf::Text m_aiTimeText{ m_arialFont };
	/// @brief Visual indicator for AI's last move (from position)
	sf::RectangleShape m_aiMoveFromIndicator;
	/// @brief Visual indicator for AI's last move (to position)
	sf::RectangleShape m_aiMoveToIndicator;
	/// @brief Flag to show/hide AI analyzer
	bool m_showAIAnalyzer = true;
	/// @brief Flag to track if AI has made at least one move
	bool m_aiHasMoved = false;
	/// @brief Timer for tracking AI calculation time
	sf::Clock m_aiCalculationClock;
	/// @brief Last AI calculation time in milliseconds
	float m_lastAICalculationTime = 0.0f;

	// Main menu
	/// @brief Button rectangle for Player vs AI option
	sf::RectangleShape m_btnPvAI;
	/// @brief Button rectangle for Player vs Player option
	sf::RectangleShape m_btnPvP;
	/// @brief Text label for Player vs AI button
	sf::Text m_btnPvAIText{ m_arialFont };
	/// @brief Text label for Player vs Player button
	sf::Text m_btnPvPText{ m_arialFont };
	/// @brief Flag indicating if the main menu should be displayed
	bool m_showMenu = true;

	// AI
	/// @brief AI player instance for computer opponent
	AI m_ai;
	/// @brief Flag indicating if this is an AI game (false = PvP, true = PvAI)
	bool m_isAIGame = true;  // false = PvP, true = PvAI
	/// @brief Flag indicating if AI is currently thinking/calculating
	bool m_aiThinking = false;
	/// @brief Elapsed time for AI thinking animation
	double m_aiThinkTime = 0.0;
	/// @brief Duration for AI to "think" before making a move (for realism)
	double m_aiThinkDuration = 1.0;  // 1 second think time for AI
};

