#include "Game.h"
#include <iostream>

// Our target FPS
static double const FPS{ 60.0f };

////////////////////////////////////////////////////////////
Game::Game()
	: m_window(sf::VideoMode(sf::Vector2u(ScreenSize::s_width, ScreenSize::s_height), 32), "SFML Playground", sf::Style::Default)
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
}

void Game::setupPieces(vector<Piece>& pieces, int row, const float cellSize, Vector2f startPos, bool isP1)
{
	if (row == 0) pieces.push_back(Piece(Piece::Type::Frog, m_frogTexture, cellSize, startPos, isP1));
	else if (row == 1) pieces.push_back(Piece(Piece::Type::Snake, m_snakeTexture, cellSize, startPos, isP1));
	else pieces.push_back(Piece(Piece::Type::Donkey, m_donkeyTexture, cellSize, startPos, isP1));
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
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
	{
		switch (keyPressed->scancode)
		{
		case sf::Keyboard::Scancode::Escape:
			m_window.close();
			break;
		case sf::Keyboard::Scancode::Up:
			// Up key was pressed...
			break;
		default:
			break;
		}
	}
}
#pragma endregion


////////////////////////////////////////////////////////////
void Game::update(double dt)
{

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


#pragma region Always On Top
#ifdef TEST_FPS
	m_window.draw(x_updateFPS); //ups is 60 and dps is 61
	m_window.draw(x_drawFPS);
#endif

#pragma endregion

	m_window.display();
}


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
#pragma endregion

