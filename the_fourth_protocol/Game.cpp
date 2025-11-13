#include "Game.h"
#include <iostream>
#include <sstream>

// Our target FPS
static double const FPS{ 60.0f };

////////////////////////////////////////////////////////////
Game::Game()
	: m_window(sf::VideoMode(sf::Vector2u(ScreenSize::s_width, ScreenSize::s_height), 32), "The Fourth Protocol", sf::Style::Default)
{
	init();
}

////////////////////////////////////////////////////////////
void Game::init()
{
#pragma region NOT GOING TO NEED TO SEE THESE

	// Really only necessary is our target FPS is greater than 60.
	m_window.setVerticalSyncEnabled(true);
	
	if (!m_arialFont.openFromFile("ASSETS/FONTS/ariblk.ttf"))
	{
		std::cout << "Error loading font file";
	}

#ifdef TEST_FPS
	x_updateFPS.setFont(m_arialFont);
	x_updateFPS.setPosition(sf::Vector2f(20, 300));
	x_updateFPS.setCharacterSize(24);
	x_updateFPS.setFillColor(sf::Color::White);
	x_drawFPS.setFont(m_arialFont);
	x_drawFPS.setPosition(sf::Vector2f(20, 350));
	x_drawFPS.setCharacterSize(24);
	x_drawFPS.setFillColor(sf::Color::White);
#endif
#pragma endregion

	m_board.resize(m_gridRows);
	for (int i = 0; i < m_gridRows; ++i) {
		m_board[i].resize(m_gridCols, nullptr);
	}

	const float cellSizeXY = 100.0f;
	const int gridSizeXY = m_gridRows * cellSizeXY;
	const float x0 = 0.5f * (static_cast<float>(ScreenSize::s_width) - gridSizeXY); // grid x origin pos
	const float y0 = 0.5f * (static_cast<float>(ScreenSize::s_height) - gridSizeXY); // grid y origin pos

	m_grid.resize(m_gridRows * m_gridCols);
	m_p1Grid.resize(5);
	m_p2Grid.resize(m_gridRows * 1);
	for(int row = 0; row < m_gridRows; ++row) {
		// please remember that this is only being done here becuase there's only 1 column per player. If this increases for any reason, create a new nested loop
		setupGrid(m_p1Grid, row, 0, 1, cellSizeXY, x0 - cellSizeXY - 75.0f, y0, Color::Red);
		setupPieces(m_p1Pieces, row, cellSizeXY, m_p1Grid[row].getPosition(), true);

		setupGrid(m_p2Grid, row, 0, 1, cellSizeXY, x0 + gridSizeXY + 75.0f, y0, Color::Blue);
		setupPieces(m_p2Pieces, row, cellSizeXY, m_p2Grid[row].getPosition(), false);

		for(int col = 0; col < m_gridCols; ++col) {
			setupGrid(m_grid, row, col, m_gridCols, cellSizeXY, x0, y0, Color::White);
		}
	}

#pragma region text and button setup


	m_statusText.setFont(m_arialFont);
	m_statusText.setCharacterSize(28);
	m_statusText.setFillColor(sf::Color::White);
	m_statusText.setPosition({ 20.0f, 20.0f });

	m_instructionText.setFont(m_arialFont);
	m_instructionText.setCharacterSize(18);
	m_instructionText.setFillColor(sf::Color::Cyan);
	m_instructionText.setPosition({ 20.0f, 60.0f });

	m_statusText.setString("Player 1 - PLACEMENT PHASE");
	m_instructionText.setString("Click and drag pieces to place them on the board");

	// Setup main menu buttons (positioned relative to grid origin)
	{
		float btnW = 220.0f;
		float btnH = 64.0f;
		float centerX = x0 + 0.5f * static_cast<float>(gridSizeXY);
		float btnY = y0 - 120.0f;

		m_btnPvP.setSize({ btnW, btnH });
		m_btnPvAI.setSize({ btnW, btnH });
		m_btnPvP.setPosition({ centerX - btnW - 10.0f, btnY });
		m_btnPvAI.setPosition({ centerX + 10.0f, btnY });
		m_btnPvP.setFillColor(sf::Color(100, 149, 237));
		m_btnPvAI.setFillColor(sf::Color(70, 130, 180));
		m_btnPvP.setOutlineThickness(2.0f);
		m_btnPvAI.setOutlineThickness(2.0f);
		m_btnPvP.setOutlineColor(sf::Color::White);
		m_btnPvAI.setOutlineColor(sf::Color::White);

		m_btnPvPText.setFont(m_arialFont);
		m_btnPvAIText.setFont(m_arialFont);
		m_btnPvPText.setCharacterSize(20);
		m_btnPvAIText.setCharacterSize(20);
		m_btnPvPText.setFillColor(sf::Color::White);
		m_btnPvAIText.setFillColor(sf::Color::White);
		m_btnPvPText.setString("Player");
		m_btnPvAIText.setString("AI");

		// center texts inside buttons
		auto pb = m_btnPvPText.getLocalBounds();
		auto pa = m_btnPvAIText.getLocalBounds();
	
	auto posP = m_btnPvP.getPosition();
		auto posA = m_btnPvAI.getPosition();
		m_btnPvPText.setPosition({ posP.x + (btnW - pb.size.x) / 2.0f - pb.position.x, posP.y + (btnH - pb.size.y) / 2.0f - pb.position.y });
		m_btnPvAIText.setPosition({ posA.x + (btnW - pa.size.x) / 2.0f - pa.position.x, posA.y + (btnH - pa.size.y) / 2.0f - pa.position.y });

		m_showMenu = true;
	}
#pragma endregion

}

void Game::setupPieces(vector<Piece>& pieces, int row, const float cellSize, Vector2f startPos, bool isP1)
{
	if (row == 0) pieces.push_back(Piece(Piece::Type::Frog, m_frogTexture, cellSize, startPos, isP1));
	else if (row == 1) pieces.push_back(Piece(Piece::Type::Snake, m_snakeTexture, cellSize, startPos, isP1));
	else pieces.push_back(Piece(Piece::Type::Donkey, m_donkeyTexture, cellSize, startPos, isP1));
}

void Game::updateBoard() {
	for (int row = 0; row < m_gridRows; ++row) {
		for (int col = 0; col < m_gridCols; ++col) {
			m_board[row][col] = nullptr;
		}
	}

	for (auto& piece : m_p1Pieces) {
		if (piece.getGridRow() >= 0 && piece.getGridCol() >= 0) {
			m_board[piece.getGridRow()][piece.getGridCol()] = &piece;
		}
	}

	for (auto& piece : m_p2Pieces) {
		if (piece.getGridRow() >= 0 && piece.getGridCol() >= 0) {
			m_board[piece.getGridRow()][piece.getGridCol()] = &piece;
		}
	}
}

Piece* Game::getPieceAtGridPosition(int row, int col) {
	if (row < 0 || row >= m_gridRows || col < 0 || col >= m_gridCols) return nullptr;
	return m_board[row][col];
}

bool Game::checkWinCondition(bool isPlayer1) {
	// Horizontal
	for (int row = 0; row < m_gridRows; ++row) {
		for (int col = 0; col < m_gridCols - 3; ++col) {
			if (m_board[row][col] && m_board[row][col]->isPlayer1() == isPlayer1 &&
				m_board[row][col + 1] && m_board[row][col + 1]->isPlayer1() == isPlayer1 &&
				m_board[row][col + 2] && m_board[row][col + 2]->isPlayer1() == isPlayer1 &&
				m_board[row][col + 3] && m_board[row][col + 3]->isPlayer1() == isPlayer1) {
				return true;
			}
		}
	}

	// Vertical
	for (int col = 0; col < m_gridCols; ++col) {
		for (int row = 0; row < m_gridRows - 3; ++row) {
			if (m_board[row][col] && m_board[row][col]->isPlayer1() == isPlayer1 &&
				m_board[row + 1][col] && m_board[row + 1][col]->isPlayer1() == isPlayer1 &&
				m_board[row + 2][col] && m_board[row + 2][col]->isPlayer1() == isPlayer1 &&
				m_board[row + 3][col] && m_board[row + 3][col]->isPlayer1() == isPlayer1) {
				return true;
			}
		}
	}

	// Diagonal (top-left to bottom-right)
	for (int row = 0; row < m_gridRows - 3; ++row) {
		for (int col = 0; col < m_gridCols - 3; ++col) {
			if (m_board[row][col] && m_board[row][col]->isPlayer1() == isPlayer1 &&
				m_board[row + 1][col + 1] && m_board[row + 1][col + 1]->isPlayer1() == isPlayer1 &&
				m_board[row + 2][col + 2] && m_board[row + 2][col + 2]->isPlayer1() == isPlayer1 &&
				m_board[row + 3][col + 3] && m_board[row + 3][col + 3]->isPlayer1() == isPlayer1) {
				return true;
			}
		}
	}

	// Diagonal (top-right to bottom-left)
	for (int row = 0; row < m_gridRows - 3; ++row) {
		for (int col = 3; col < m_gridCols; ++col) {
			if (m_board[row][col] && m_board[row][col]->isPlayer1() == isPlayer1 &&
				m_board[row + 1][col - 1] && m_board[row + 1][col - 1]->isPlayer1() == isPlayer1 &&
				m_board[row + 2][col - 2] && m_board[row + 2][col - 2]->isPlayer1() == isPlayer1 &&
				m_board[row + 3][col - 3] && m_board[row + 3][col - 3]->isPlayer1() == isPlayer1) {
				return true;
			}
		}
	}

	return false;
}

bool Game::isValidPlacement(int row, int col) {
	if (row < 0 || row >= m_gridRows || col < 0 || col >= m_gridCols) return false;
	if (m_board[row][col] != nullptr) return false;
	return true;
}

bool Game::placePiece(Piece* piece, int row, int col) {
	if (!isValidPlacement(row, col)) return false;

	piece->setGridPosition(row, col);
	m_board[row][col] = piece;

	if (piece->isPlayer1()) {
		m_p1PiecesPlaced++;
	} else {
		m_p2PiecesPlaced++;
	}

	if (checkWinCondition(piece->isPlayer1())) {
		m_gamePhase = GamePhase::GameOver;
		m_winner = piece;
		std::string winnerText = piece->isPlayer1() ? "Player 1 Wins!" : "Player 2 Wins!";
		m_statusText.setString(winnerText + " - 4 in a row!");
		m_instructionText.setString("Close window to exit");
		return true;
	}

	return true;
}

bool Game::movePiece(Piece* piece, int fromRow, int fromCol, int toRow, int toCol) {
	if (!piece->isValidMove(toRow, toCol, m_board, m_gridRows)) {
		return false;
	}

	// Update board
	m_board[fromRow][fromCol] = nullptr;
	m_board[toRow][toCol] = piece;
	piece->setGridPosition(toRow, toCol);

	// Check for win
	if (checkWinCondition(piece->isPlayer1())) {
		m_gamePhase = GamePhase::GameOver;
		m_winner = piece;
		std::string winnerText = piece->isPlayer1() ? "Player 1 Wins!" : "Player 2 Wins!";
		m_statusText.setString(winnerText + " - 4 in a row!");
		m_instructionText.setString("Close window to exit");
		return true;
	}

	return true;
}

void Game::endTurn() {
	if (m_gamePhase == GamePhase::GameOver) return;
	m_isPlayer1Turn = !m_isPlayer1Turn;

	std::string phaseText = m_gamePhase == GamePhase::Placement ? "PLACEMENT" : "MOVEMENT";
	std::string playerText = m_isPlayer1Turn ? "Player 1" : (m_isAIGame ? "AI (Player 2)" : "Player 2");
	m_statusText.setString(playerText + " - " + phaseText + " PHASE");

	if (m_isAIGame && !m_isPlayer1Turn && m_gamePhase == GamePhase::Movement) {
		m_instructionText.setString("AI is thinking...");
	}

	if (m_gamePhase == GamePhase::Placement && m_p1PiecesPlaced == 5 && m_p2PiecesPlaced == 5) {
		m_gamePhase = GamePhase::Movement;
		m_isPlayer1Turn = true;
		m_statusText.setString("Player 1 - MOVEMENT PHASE");
		m_instructionText.setString("Click and drag pieces to move them");
	}
}

#pragma region NEVER GONNA TOUCH 
////////////////////////////////////////////////////////////
void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	
	sf::Time timePerFrame = sf::seconds(1.0f / FPS); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame.asMilliseconds()); //60 fps
#ifdef TEST_FPS
			x_secondTime += timePerFrame;
			x_updateFrameCount++;
			if (x_secondTime.asSeconds() > 1)
			{
				std::string updatesPS = "UPS " + std::to_string(x_updateFrameCount - 1);
				x_updateFPS.setString(updatesPS);
				std::string drawsPS = "DPS " + std::to_string(x_drawFrameCount);
				x_drawFPS.setString(drawsPS);
				x_updateFrameCount = 0;
				x_drawFrameCount = 0;
				x_secondTime = sf::Time::Zero;
			}
#endif
		}
		render(); // as many as possible
#ifdef TEST_FPS
		x_drawFrameCount++;
#endif
	}
}

////////////////////////////////////////////////////////////
void Game::processEvents()
{
    while (const std::optional event = m_window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            m_window.close();
        }
        processGameEvents(*event);
    }
}


////////////////////////////////////////////////////////////
void Game::processGameEvents(const sf::Event& event)
{
	// Always allow Escape to close the window
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
	{
		if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
			m_window.close();
			return;
		}
	}

	// If the main menu is visible, handle menu clicks and ignore other game events
	if (m_showMenu)
	{
		if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
		{
			if (mousePressed->button == sf::Mouse::Button::Left)
			{
				Vector2f mousePos = m_window.mapPixelToCoords(
					Vector2i(mousePressed->position.x, mousePressed->position.y)
				);
				if (m_btnPvAI.getGlobalBounds().contains(mousePos)) {
					m_isAIGame = true;
					m_showMenu = false;
					m_statusText.setString("Player 1 - PLACEMENT PHASE");
					m_instructionText.setString("Click and drag pieces to place them on the board");
					return;
				}
				if (m_btnPvP.getGlobalBounds().contains(mousePos)) {
					m_isAIGame = false;
					m_showMenu = false;
					m_statusText.setString("Player 1 - PLACEMENT PHASE");
					m_instructionText.setString("Click and drag pieces to place them on the board");
					return;
				}
			}
		}
		return; // ignore other events while in menu
	}

	if (m_gamePhase == GamePhase::GameOver) return;

	if (m_isAIGame && !m_isPlayer1Turn && (m_gamePhase == GamePhase::Placement || m_gamePhase == GamePhase::Movement)) {
		return;
	}

	if (const auto* keyPressed2 = event.getIf<sf::Event::KeyPressed>())
	{
		switch (keyPressed2->scancode)
		{
		case sf::Keyboard::Scancode::Space:
			if (!m_isAIGame || m_isPlayer1Turn) {
				endTurn();
			}
			break;
		default:
			break;
		}
	}

	if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (mousePressed->button == sf::Mouse::Button::Left)
		{
			Vector2f mousePos = m_window.mapPixelToCoords(
				Vector2i(mousePressed->position.x, mousePressed->position.y)
			);

			if (m_isPlayer1Turn) {
				for (auto& piece : m_p1Pieces) {
					if (piece.contains(mousePos)) {
						m_selectedPiece = &piece;
						m_isDragging = true;
						m_dragOffset = piece.getPosition() - mousePos;
						piece.setSelected(true);
						piece.saveOriginalPosition();
						return;
					}
				}
			} else if (!m_isAIGame) {
				for (auto& piece : m_p2Pieces) {
					if (piece.contains(mousePos)) {
						m_selectedPiece = &piece;
						m_isDragging = true;
						m_dragOffset = piece.getPosition() - mousePos;
						piece.setSelected(true);
						piece.saveOriginalPosition();
						return;
					}
				}
			}
		}
	}

	if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>())
	{
		if (mouseReleased->button == sf::Mouse::Button::Left && m_isDragging)
		{
			Vector2f mousePos = m_window.mapPixelToCoords(
				Vector2i(mouseReleased->position.x, mouseReleased->position.y)
			);

			snapToGrid(mousePos);

			if (m_selectedPiece) {
				m_selectedPiece->setSelected(false);
			}
			m_selectedPiece = nullptr;
			m_isDragging = false;
		}
	}

	if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>())
	{
		if (m_isDragging && m_selectedPiece)
		{
			Vector2f mousePos = m_window.mapPixelToCoords(
				Vector2i(mouseMoved->position.x, mouseMoved->position.y)
			);
			m_selectedPiece->setPosition(mousePos + m_dragOffset);
		}
	}
}
#pragma endregion


////////////////////////////////////////////////////////////
void Game::update(double dt)
{
	if (m_isAIGame && !m_isPlayer1Turn && m_gamePhase == GamePhase::Movement && !m_aiThinking) {
		m_aiThinking = true;
		m_aiThinkTime = 0.0;
	}

	if (m_aiThinking) {
		m_aiThinkTime += dt;
		if (m_aiThinkTime >= m_aiThinkDuration) {
			executeAIMove();
			m_aiThinking = false;
		}
	}

	if (m_isAIGame && !m_isPlayer1Turn && m_gamePhase == GamePhase::Placement) {
		executeAIMove();
	}
}

////////////////////////////////////////////////////////////
void Game::render()
{
	m_window.clear(sf::Color(0, 0, 0, 0));

	for (const auto& cell : m_grid)
		m_window.draw(cell);
	for (const auto& cell : m_p1Grid)
		m_window.draw(cell);
	for (const auto& cell : m_p2Grid)
		m_window.draw(cell);

	for (const auto& piece : m_p1Pieces)
		piece.draw(m_window);
	for (const auto& piece : m_p2Pieces)
		piece.draw(m_window);

	if (m_gamePhase == GamePhase::GameOver) {
		sf::RectangleShape overlay({ static_cast<float>(ScreenSize::s_width), static_cast<float>(ScreenSize::s_height) });
		overlay.setFillColor(sf::Color(0, 0, 0, 180));
		m_window.draw(overlay);
	}

	m_window.draw(m_statusText);
	m_window.draw(m_instructionText);

#ifdef TEST_FPS
	m_window.draw(x_updateFPS);
	m_window.draw(x_drawFPS);
#endif

	if (m_showMenu) {
		sf::RectangleShape menuOverlay({ static_cast<float>(ScreenSize::s_width), static_cast<float>(ScreenSize::s_height) });
		menuOverlay.setFillColor(sf::Color(0, 0, 0, 200));
		m_window.draw(menuOverlay);
		m_window.draw(m_btnPvP);
		m_window.draw(m_btnPvAI);
		m_window.draw(m_btnPvPText);
		m_window.draw(m_btnPvAIText);
	}

	m_window.display();
}
#pragma region AI

void Game::executeAIMove()
{
	if (m_gamePhase == GamePhase::GameOver) return;

	Move aiMove = m_ai.findBestMove(m_board, m_p2Pieces, m_p1Pieces, m_gridRows, 
		m_gamePhase == GamePhase::Placement);

	if (aiMove.pieceIndex >= 0 && aiMove.pieceIndex < m_p2Pieces.size()) {
		applyAIMove(aiMove);
	}
}

void Game::applyAIMove(const Move& move)
{
	Piece* piece = &m_p2Pieces[move.pieceIndex];

	if (m_gamePhase == GamePhase::Placement) {
		if (placePiece(piece, move.toRow, move.toCol)) {
			const float cellSizeXY = 100.0f;
			const float gridSizeXY = m_gridRows * cellSizeXY;
			const float x0 = 0.5f * (static_cast<float>(ScreenSize::s_width) - gridSizeXY);
			const float y0 = 0.5f * (static_cast<float>(ScreenSize::s_height) - gridSizeXY);

			Vector2f cellPos = {
				x0 + static_cast<float>(move.toCol * cellSizeXY),
				y0 + static_cast<float>(move.toRow * cellSizeXY)
			};
			piece->setPosition(cellPos);
			endTurn();
		}
	} else if (m_gamePhase == GamePhase::Movement) {
		if (movePiece(piece, move.fromRow, move.fromCol, move.toRow, move.toCol)) {
			const float cellSizeXY = 100.0f;
			const float gridSizeXY = m_gridRows * cellSizeXY;
			const float x0 = 0.5f * (static_cast<float>(ScreenSize::s_width) - gridSizeXY);
			const float y0 = 0.5f * (static_cast<float>(ScreenSize::s_height) - gridSizeXY);

			Vector2f cellPos = {
				x0 + static_cast<float>(move.toCol * cellSizeXY),
				y0 + static_cast<float>(move.toRow * cellSizeXY)
			};
			piece->setPosition(cellPos);
			updateBoard();
			endTurn();
		}
	}
}
#pragma endregion

#pragma region Helpers
void Game::setupGrid(vector<RectangleShape>& grid, int row, int col, int numCols, const float cellSizeXY, const float x0, const float y0, sf::Color outlineColour)
{
	grid[row * numCols + col].setSize(Vector2f(cellSizeXY, cellSizeXY));
	grid[row * numCols + col].setOutlineColor(outlineColour);
	grid[row * numCols + col].setFillColor(sf::Color::Black);
	grid[row * numCols + col].setOutlineThickness(2.0f);
	grid[row * numCols + col].setPosition(Vector2f(
		x0 + static_cast<float>(col * cellSizeXY),
		y0 + static_cast<float>(row * cellSizeXY)
	));
}

void Game::snapToGrid(Vector2f mousePos)
{
	if (!m_selectedPiece) return;

	bool foundValidCell = false;

	for (int row = 0; row < m_gridRows; ++row) {
		for (int col = 0; col < m_gridCols; ++col) {
			const auto& cell = m_grid[row * m_gridCols + col];
			if (cell.getGlobalBounds().contains(mousePos))
			{
				int prevRow = m_selectedPiece->getGridRow();
				int prevCol = m_selectedPiece->getGridCol();

				if (m_gamePhase == GamePhase::Placement) {
					if (placePiece(m_selectedPiece, row, col)) {
						Vector2f cellPos = cell.getPosition();
						m_selectedPiece->setPosition(cellPos);
						foundValidCell = true;
						endTurn();
						return;
					}
				} else if (m_gamePhase == GamePhase::Movement) {
					// It's a movement phase
					if (prevRow >= 0 && prevCol >= 0) {
						if (movePiece(m_selectedPiece, prevRow, prevCol, row, col)) {
							Vector2f cellPos = cell.getPosition();
							m_selectedPiece->setPosition(cellPos);
							updateBoard();
							foundValidCell = true;
							endTurn();
							return;
						}
					}
				}
			}
		}
	}

	if (!foundValidCell) {
		m_selectedPiece->restoreOriginalPosition();
	}
}

#pragma endregion
