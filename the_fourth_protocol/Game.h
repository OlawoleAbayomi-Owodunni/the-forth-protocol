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

struct ScreenSize
{
public:
	static const int s_width{ 1440 };

	static const int s_height{ 900 };
};

class Game
{
public:
	enum class GamePhase {
		Placement,		// Players placing pieces
		Movement,		// Players moving pieces
		GameOver		// Game finished
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
	/// <param name="time">update delta time</param>
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

	// Font used for all text
	sf::Font m_arialFont{ "ASSETS/FONTS/ariblk.ttf" };
	// main window
	sf::RenderWindow m_window;

#ifdef TEST_FPS
	sf::Text x_updateFPS{ m_arialFont };	// text used to display updates per second.
	sf::Text x_drawFPS{  m_arialFont };	// text used to display draw calls per second.
	sf::Time x_secondTime{ sf::Time::Zero };			// counter used to establish when a second has passed.
	int x_updateFrameCount{ 0 };						// updates per second counter.
	int x_drawFrameCount{ 0 };							// draws per second counter.
#endif // TEST_FPS
#pragma endregion

private:
	/// @brief Sets up a grid of RectangleShapes
	void setupGrid(vector<RectangleShape>& grid, int row, int col, int numCols, const float cellSizeXY, const float x0, const float y0, sf::Color outlineColour);
	/// @brief Sets up pieces for a player
	void setupPieces(vector<Piece>& pieces, int row, const float cellSizeXY, Vector2f startPos, bool isP1);

	void snapToGrid(Vector2f mousePos);

	// Game logic methods
	bool isValidPlacement(int row, int col);
	bool placePiece(Piece* piece, int row, int col);
	bool movePiece(Piece* piece, int fromRow, int fromCol, int toRow, int toCol);
	void endTurn();
	bool checkWinCondition(bool isPlayer1);
	Piece* getPieceAtGridPosition(int row, int col);
	void updateBoard();

	// AI methods
	void executeAIMove();
	void applyAIMove(const Move& move);

	// Board state
	vector<vector<Piece*>> m_board;

	vector<RectangleShape> m_grid;
	int m_gridRows{ 5 };
	int m_gridCols{ 5 };

	vector<RectangleShape> m_p1Grid;
	vector<RectangleShape> m_p2Grid;

	Texture m_frogTexture{ "ASSETS/IMAGES/frog.png" };
	Texture m_snakeTexture{ "ASSETS/IMAGES/snake.png" };
	Texture m_donkeyTexture{ "ASSETS/IMAGES/donkey.png" };

	vector<Piece> m_p1Pieces;
	vector<Piece> m_p2Pieces;

	Piece* m_selectedPiece = nullptr;
	Vector2f m_dragOffset;
	bool m_isDragging = false;

	// Game state
	GamePhase m_gamePhase = GamePhase::Placement;
	bool m_isPlayer1Turn = true;  // Player 1 starts
	int m_p1PiecesPlaced = 0;
	int m_p2PiecesPlaced = 0;
	Piece* m_winner = nullptr;

	sf::Text m_statusText{ m_arialFont };
	sf::Text m_instructionText{ m_arialFont };

	// Main menu
	sf::RectangleShape m_btnPvAI;
	sf::RectangleShape m_btnPvP;
	sf::Text m_btnPvAIText{ m_arialFont };
	sf::Text m_btnPvPText{ m_arialFont };
	bool m_showMenu = true;

	// AI
	AI m_ai;
	bool m_isAIGame = true;  // false = PvP, true = PvAI
	bool m_aiThinking = false;
	double m_aiThinkTime = 0.0;
	double m_aiThinkDuration = 1.0;  // 1 second think time for AI
};

