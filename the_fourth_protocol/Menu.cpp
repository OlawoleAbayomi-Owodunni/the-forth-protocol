#include "Menu.h"
#include <cmath>

////////////////////////////////////////////////////////////
// MenuButton Implementation
////////////////////////////////////////////////////////////

MenuButton::MenuButton(const sf::Vector2f& position, const sf::Vector2f& size,
	const std::string& text, const sf::Font& font,
	const sf::Color& normalColor, const sf::Color& hoverColor)
	: m_font(font)
	, m_normalColor(normalColor)
	, m_hoverColor(hoverColor)
	, m_disabledColor(sf::Color(80, 80, 80))
	, m_isHovered(false)
	, m_enabled(true)
{
	// Setup button shape
	m_shape.setSize(size);
	m_shape.setPosition(position);
	m_shape.setFillColor(normalColor);
	m_shape.setOutlineThickness(3.0f);
	m_shape.setOutlineColor(sf::Color::White);

	// Setup text
	m_text.setString(text);
	m_text.setCharacterSize(22);
	m_text.setFillColor(sf::Color::White);

	// Center text in button
	sf::FloatRect textBounds = m_text.getLocalBounds();
	m_text.setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x / 2.0f,
		textBounds.position.y + textBounds.size.y / 2.0f));
	m_text.setPosition(sf::Vector2f(position.x + size.x / 2.0f, position.y + size.y / 2.0f));
}

void MenuButton::update(const sf::Vector2f& mousePos)
{
	if (!m_enabled) {
		m_isHovered = false;
		return;
	}

	m_isHovered = contains(mousePos);

	if (m_isHovered) {
		m_shape.setFillColor(m_hoverColor);
		m_shape.setOutlineThickness(4.0f);
		m_shape.setOutlineColor(sf::Color(255, 255, 100));
	}
	else {
		m_shape.setFillColor(m_normalColor);
		m_shape.setOutlineThickness(3.0f);
		m_shape.setOutlineColor(sf::Color::White);
	}
}

void MenuButton::draw(sf::RenderWindow& window)
{
	window.draw(m_shape);
	window.draw(m_text);
}

bool MenuButton::contains(const sf::Vector2f& point) const
{
	return m_shape.getGlobalBounds().contains(point);
}

void MenuButton::setEnabled(bool enabled)
{
	m_enabled = enabled;
	if (!enabled) {
		m_shape.setFillColor(m_disabledColor);
		m_text.setFillColor(sf::Color(150, 150, 150));
	}
	else {
		m_shape.setFillColor(m_normalColor);
		m_text.setFillColor(sf::Color::White);
	}
}

void MenuButton::setText(const std::string& text)
{
	m_text.setString(text);
	sf::FloatRect textBounds = m_text.getLocalBounds();
	sf::Vector2f buttonPos = m_shape.getPosition();
	sf::Vector2f buttonSize = m_shape.getSize();
	m_text.setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x / 2.0f,
		textBounds.position.y + textBounds.size.y / 2.0f));
	m_text.setPosition(sf::Vector2f(buttonPos.x + buttonSize.x / 2.0f, buttonPos.y + buttonSize.y / 2.0f));
}

void MenuButton::setPosition(const sf::Vector2f& position)
{
	m_shape.setPosition(position);
	sf::FloatRect textBounds = m_text.getLocalBounds();
	sf::Vector2f buttonSize = m_shape.getSize();
	m_text.setPosition(sf::Vector2f(position.x + buttonSize.x / 2.0f, position.y + buttonSize.y / 2.0f));
}

////////////////////////////////////////////////////////////
// Menu Implementation
////////////////////////////////////////////////////////////

Menu::Menu(const sf::Font& font, const sf::Vector2f& windowSize)
	: m_font(font)
	, m_windowSize(windowSize)
	, m_state(State::Main)
	, m_pvpButton(sf::Vector2f(0, 0), sf::Vector2f(280, 70), "Player vs Player", font)
	, m_pvaiButton(sf::Vector2f(0, 0), sf::Vector2f(280, 70), "Player vs AI", font)
	, m_aivsaiButton(sf::Vector2f(0, 0), sf::Vector2f(280, 70), "AI vs AI", font, sf::Color(130, 70, 180), sf::Color(160, 100, 220))
	, m_exitButton(sf::Vector2f(0, 0), sf::Vector2f(280, 70), "Exit Game", font, sf::Color(180, 70, 70), sf::Color(220, 100, 100))
	, m_easyButton(sf::Vector2f(0, 0), sf::Vector2f(280, 70), "Easy", font, sf::Color(70, 180, 70), sf::Color(100, 220, 100))
	, m_mediumButton(sf::Vector2f(0, 0), sf::Vector2f(280, 70), "Medium", font, sf::Color(180, 140, 70), sf::Color(220, 180, 100))
	, m_hardButton(sf::Vector2f(0, 0), sf::Vector2f(280, 70), "Hard", font, sf::Color(180, 70, 70), sf::Color(220, 100, 100))
	, m_backButton(sf::Vector2f(0, 0), sf::Vector2f(280, 70), "Back", font, sf::Color(100, 100, 100), sf::Color(140, 140, 140))
	, m_balancedButton(sf::Vector2f(0, 0), sf::Vector2f(280, 60), "Balanced", font)
	, m_centerButton(sf::Vector2f(0, 0), sf::Vector2f(280, 60), "Favor Center", font)
	, m_edgesButton(sf::Vector2f(0, 0), sf::Vector2f(280, 60), "Favor Edges", font)
	, m_diagonalButton(sf::Vector2f(0, 0), sf::Vector2f(280, 60), "Favor Diagonal", font)
	, m_aggressiveButton(sf::Vector2f(0, 0), sf::Vector2f(280, 60), "Aggressive", font, sf::Color(180, 70, 70), sf::Color(220, 100, 100))
	, m_defensiveButton(sf::Vector2f(0, 0), sf::Vector2f(280, 60), "Defensive", font, sf::Color(70, 180, 70), sf::Color(100, 220, 100))
	, m_backFromStrategyButton(sf::Vector2f(0, 0), sf::Vector2f(280, 60), "Back", font, sf::Color(100, 100, 100), sf::Color(140, 140, 140))
	, m_restartButton(sf::Vector2f(0, 0), sf::Vector2f(280, 70), "Restart Game", font)
	, m_mainMenuButton(sf::Vector2f(0, 0), sf::Vector2f(280, 70), "Main Menu", font)
	, m_toggleMenuButton(sf::Vector2f(0, 0), sf::Vector2f(180, 50), "Hide Menu", font, sf::Color(100, 100, 100), sf::Color(140, 140, 140))
	, m_pvpSelected(false)
	, m_pvaiSelected(false)
	, m_aivsaiSelected(false)
	, m_selectedDifficulty(Difficulty::Medium)
	, m_aiStrategy(Strategy::Balanced)
	, m_ai1Strategy(Strategy::Balanced)
	, m_ai2Strategy(Strategy::Balanced)
	, m_startGame(false)
	, m_exitGame(false)
	, m_menuCollapsed(false)
{
	// Setup background overlay
	m_background.setSize(windowSize);
	m_background.setPosition(sf::Vector2f(0.0f, 0.0f));
	m_background.setFillColor(sf::Color(0, 0, 0, 200));

	// Setup menu panel
	float panelWidth = 600.0f;
	float panelHeight = 550.0f;
	m_panel.setSize(sf::Vector2f(panelWidth, panelHeight));
	m_panel.setPosition(sf::Vector2f((windowSize.x - panelWidth) / 2.0f, (windowSize.y - panelHeight) / 2.0f));
	m_panel.setFillColor(sf::Color(30, 30, 50, 250));
	m_panel.setOutlineThickness(5.0f);
	m_panel.setOutlineColor(sf::Color(100, 149, 237));

	// Setup title text
	m_titleText.setCharacterSize(48);
	m_titleText.setFillColor(sf::Color(100, 200, 255));
	m_titleText.setString("The Fourth Protocol");
	sf::FloatRect titleBounds = m_titleText.getLocalBounds();
	m_titleText.setOrigin(sf::Vector2f(titleBounds.position.x + titleBounds.size.x / 2.0f, 
		titleBounds.position.y + titleBounds.size.y / 2.0f));
	m_titleText.setPosition(sf::Vector2f(windowSize.x / 2.0f, m_panel.getPosition().y + 60.0f));

	// Setup subtitle text
	m_subtitleText.setCharacterSize(20);
	m_subtitleText.setFillColor(sf::Color(180, 180, 180));
	m_subtitleText.setString("Select Game Mode");
	sf::FloatRect subtitleBounds = m_subtitleText.getLocalBounds();
	m_subtitleText.setOrigin(sf::Vector2f(subtitleBounds.position.x + subtitleBounds.size.x / 2.0f,
		subtitleBounds.position.y + subtitleBounds.size.y / 2.0f));
	m_subtitleText.setPosition(sf::Vector2f(windowSize.x / 2.0f, m_panel.getPosition().y + 120.0f));

	// Setup game over text
	m_gameOverText.setCharacterSize(35);
	m_gameOverText.setFillColor(sf::Color(255, 215, 0));

	initMainMenu();
	initDifficultyMenu();
	initStrategyMenu();
	initGameOverMenu();
}

void Menu::initMainMenu()
{
	float centerX = m_windowSize.x / 2.0f;
	float startY = m_panel.getPosition().y + 180.0f;
	float buttonSpacing = 90.0f;

	// Position main menu buttons
	m_pvpButton.setPosition(sf::Vector2f(centerX - 140.0f, startY));
	m_pvaiButton.setPosition(sf::Vector2f(centerX - 140.0f, startY + buttonSpacing));
	m_aivsaiButton.setPosition(sf::Vector2f(centerX - 140.0f, startY + buttonSpacing * 2));
	m_exitButton.setPosition(sf::Vector2f(centerX - 140.0f, startY + buttonSpacing * 3));
}

void Menu::initDifficultyMenu()
{
	float centerX = m_windowSize.x / 2.0f;
	float startY = m_panel.getPosition().y + 180.0f;
	float buttonSpacing = 90.0f;

	// Position difficulty buttons
	m_easyButton.setPosition(sf::Vector2f(centerX - 140.0f, startY));
	m_mediumButton.setPosition(sf::Vector2f(centerX - 140.0f, startY + buttonSpacing));
	m_hardButton.setPosition(sf::Vector2f(centerX - 140.0f, startY + buttonSpacing * 2));
	m_backButton.setPosition(sf::Vector2f(centerX - 140.0f, startY + buttonSpacing * 3));
}

void Menu::initStrategyMenu()
{
	float centerX = m_windowSize.x / 2.0f;
	float startY = m_panel.getPosition().y + 170.0f;
	float buttonSpacing = 70.0f;

	// Position strategy buttons (6 strategies + back button)
	m_balancedButton.setPosition(sf::Vector2f(centerX - 140.0f, startY));
	m_centerButton.setPosition(sf::Vector2f(centerX - 140.0f, startY + buttonSpacing));
	m_edgesButton.setPosition(sf::Vector2f(centerX - 140.0f, startY + buttonSpacing * 2));
	m_diagonalButton.setPosition(sf::Vector2f(centerX - 140.0f, startY + buttonSpacing * 3));
	m_aggressiveButton.setPosition(sf::Vector2f(centerX - 140.0f, startY + buttonSpacing * 4));
	m_defensiveButton.setPosition(sf::Vector2f(centerX - 140.0f, startY + buttonSpacing * 5));
	m_backFromStrategyButton.setPosition(sf::Vector2f(centerX - 140.0f, startY + buttonSpacing * 6));
}

void Menu::initGameOverMenu()
{
	float centerX = m_windowSize.x / 2.0f;
	float startY = m_panel.getPosition().y + 220.0f;
	float buttonSpacing = 90.0f;

	// Position game over buttons
	m_restartButton.setPosition(sf::Vector2f(centerX - 140.0f, startY));
	m_mainMenuButton.setPosition(sf::Vector2f(centerX - 140.0f, startY + buttonSpacing));
	
	float panelRight = m_panel.getPosition().x + m_panel.getSize().x;
	float panelTop = m_panel.getPosition().y;
	m_toggleMenuButton.setPosition(sf::Vector2f(panelRight - 200.0f, panelTop + 15.0f));
}

void Menu::update(const sf::Vector2f& mousePos)
{
	switch (m_state) {
	case State::Main:
		m_pvpButton.update(mousePos);
		m_pvaiButton.update(mousePos);
		m_aivsaiButton.update(mousePos);
		m_exitButton.update(mousePos);
		break;

	case State::Difficulty:
		m_easyButton.update(mousePos);
		m_mediumButton.update(mousePos);
		m_hardButton.update(mousePos);
		m_backButton.update(mousePos);
		break;

	case State::AIStrategy:
	case State::AI1Strategy:
	case State::AI2Strategy:
		m_balancedButton.update(mousePos);
		m_centerButton.update(mousePos);
		m_edgesButton.update(mousePos);
		m_diagonalButton.update(mousePos);
		m_aggressiveButton.update(mousePos);
		m_defensiveButton.update(mousePos);
		m_backFromStrategyButton.update(mousePos);
		break;

	case State::GameOver:
		if (m_menuCollapsed) {
			m_toggleMenuButton.update(mousePos);
		} else {
			m_restartButton.update(mousePos);
			m_mainMenuButton.update(mousePos);
			m_toggleMenuButton.update(mousePos);
		}
		break;

	case State::Hidden:
		break;
	}
}

bool Menu::handleClick(const sf::Vector2f& mousePos)
{
	switch (m_state) {
	case State::Main:
		if (m_pvpButton.contains(mousePos)) {
			m_pvpSelected = true;
			m_pvaiSelected = false;
			m_aivsaiSelected = false;
			m_startGame = true;
			m_state = State::Hidden;
			return true;
		}
		else if (m_pvaiButton.contains(mousePos)) {
			m_pvpSelected = false;
			m_pvaiSelected = false;
			m_aivsaiSelected = false;
			m_state = State::Difficulty;
			m_subtitleText.setString("Select Difficulty");
			sf::FloatRect bounds = m_subtitleText.getLocalBounds();
			m_subtitleText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f,
				bounds.position.y + bounds.size.y / 2.0f));
			m_subtitleText.setPosition(sf::Vector2f(m_windowSize.x / 2.0f, m_panel.getPosition().y + 120.0f));
			return true;
		}
		else if (m_aivsaiButton.contains(mousePos)) {
			m_pvpSelected = false;
			m_pvaiSelected = false;
			m_aivsaiSelected = false;
			m_state = State::Difficulty;
			m_subtitleText.setString("Select Difficulty (AI vs AI)");
			sf::FloatRect bounds = m_subtitleText.getLocalBounds();
			m_subtitleText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f,
				bounds.position.y + bounds.size.y / 2.0f));
			m_subtitleText.setPosition(sf::Vector2f(m_windowSize.x / 2.0f, m_panel.getPosition().y + 120.0f));
			return true;
		}
		else if (m_exitButton.contains(mousePos)) {
			m_exitGame = true;
			return true;
		}
		break;

	case State::Difficulty:
		if (m_easyButton.contains(mousePos)) {
			m_selectedDifficulty = Difficulty::Easy;
			// Determine which mode was selected based on subtitle
			if (m_subtitleText.getString().find("AI vs AI") != std::string::npos) {
				m_aivsaiSelected = true;
				m_state = State::AI1Strategy;
				m_subtitleText.setString("Select AI 1 Strategy");
			} else {
				m_pvaiSelected = true;
				m_state = State::AIStrategy;
				m_subtitleText.setString("Select AI Strategy");
			}
			sf::FloatRect bounds = m_subtitleText.getLocalBounds();
			m_subtitleText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f,
				bounds.position.y + bounds.size.y / 2.0f));
			m_subtitleText.setPosition(sf::Vector2f(m_windowSize.x / 2.0f, m_panel.getPosition().y + 120.0f));
			return true;
		}
		else if (m_mediumButton.contains(mousePos)) {
			m_selectedDifficulty = Difficulty::Medium;
			if (m_subtitleText.getString().find("AI vs AI") != std::string::npos) {
				m_aivsaiSelected = true;
				m_state = State::AI1Strategy;
				m_subtitleText.setString("Select AI 1 Strategy");
			} else {
				m_pvaiSelected = true;
				m_state = State::AIStrategy;
				m_subtitleText.setString("Select AI Strategy");
			}
			sf::FloatRect bounds = m_subtitleText.getLocalBounds();
			m_subtitleText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f,
				bounds.position.y + bounds.size.y / 2.0f));
			m_subtitleText.setPosition(sf::Vector2f(m_windowSize.x / 2.0f, m_panel.getPosition().y + 120.0f));
			return true;
		}
		else if (m_hardButton.contains(mousePos)) {
			m_selectedDifficulty = Difficulty::Hard;
			if (m_subtitleText.getString().find("AI vs AI") != std::string::npos) {
				m_aivsaiSelected = true;
				m_state = State::AI1Strategy;
				m_subtitleText.setString("Select AI 1 Strategy");
			} else {
				m_pvaiSelected = true;
				m_state = State::AIStrategy;
				m_subtitleText.setString("Select AI Strategy");
			}
			sf::FloatRect bounds = m_subtitleText.getLocalBounds();
			m_subtitleText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f,
				bounds.position.y + bounds.size.y / 2.0f));
			m_subtitleText.setPosition(sf::Vector2f(m_windowSize.x / 2.0f, m_panel.getPosition().y + 120.0f));
			return true;
		}
		else if (m_backButton.contains(mousePos)) {
			m_state = State::Main;
			m_subtitleText.setString("Select Game Mode");
			sf::FloatRect bounds = m_subtitleText.getLocalBounds();
			m_subtitleText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f,
				bounds.position.y + bounds.size.y / 2.0f));
			m_subtitleText.setPosition(sf::Vector2f(m_windowSize.x / 2.0f, m_panel.getPosition().y + 120.0f));
			return true;
		}
		break;

	case State::AIStrategy:
		// Handle AI strategy selection for PvAI mode
		if (m_balancedButton.contains(mousePos)) m_aiStrategy = Strategy::Balanced;
		else if (m_centerButton.contains(mousePos)) m_aiStrategy = Strategy::FavorCenter;
		else if (m_edgesButton.contains(mousePos)) m_aiStrategy = Strategy::FavorEdges;
		else if (m_diagonalButton.contains(mousePos)) m_aiStrategy = Strategy::FavorDiagonal;
		else if (m_aggressiveButton.contains(mousePos)) m_aiStrategy = Strategy::Aggressive;
		else if (m_defensiveButton.contains(mousePos)) m_aiStrategy = Strategy::Defensive;
		else if (m_backFromStrategyButton.contains(mousePos)) {
			m_state = State::Difficulty;
			m_subtitleText.setString("Select Difficulty");
			sf::FloatRect bounds = m_subtitleText.getLocalBounds();
			m_subtitleText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f,
				bounds.position.y + bounds.size.y / 2.0f));
			m_subtitleText.setPosition(sf::Vector2f(m_windowSize.x / 2.0f, m_panel.getPosition().y + 120.0f));
			return true;
		}
		
		// If a strategy was selected (not back button), start game
		if (!m_backFromStrategyButton.contains(mousePos)) {
			m_startGame = true;
			m_state = State::Hidden;
			return true;
		}
		break;

	case State::AI1Strategy:
		// Handle AI 1 strategy selection for AIvsAI mode
		if (m_balancedButton.contains(mousePos)) m_ai1Strategy = Strategy::Balanced;
		else if (m_centerButton.contains(mousePos)) m_ai1Strategy = Strategy::FavorCenter;
		else if (m_edgesButton.contains(mousePos)) m_ai1Strategy = Strategy::FavorEdges;
		else if (m_diagonalButton.contains(mousePos)) m_ai1Strategy = Strategy::FavorDiagonal;
		else if (m_aggressiveButton.contains(mousePos)) m_ai1Strategy = Strategy::Aggressive;
		else if (m_defensiveButton.contains(mousePos)) m_ai1Strategy = Strategy::Defensive;
		else if (m_backFromStrategyButton.contains(mousePos)) {
			m_state = State::Difficulty;
			m_subtitleText.setString("Select Difficulty (AI vs AI)");
			sf::FloatRect bounds = m_subtitleText.getLocalBounds();
			m_subtitleText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f,
				bounds.position.y + bounds.size.y / 2.0f));
			m_subtitleText.setPosition(sf::Vector2f(m_windowSize.x / 2.0f, m_panel.getPosition().y + 120.0f));
			return true;
		}
		
		// If a strategy was selected (not back button), go to AI 2 strategy
		if (!m_backFromStrategyButton.contains(mousePos)) {
			m_state = State::AI2Strategy;
			m_subtitleText.setString("Select AI 2 Strategy");
			sf::FloatRect bounds = m_subtitleText.getLocalBounds();
			m_subtitleText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f,
				bounds.position.y + bounds.size.y / 2.0f));
			m_subtitleText.setPosition(sf::Vector2f(m_windowSize.x / 2.0f, m_panel.getPosition().y + 120.0f));
			return true;
		}
		break;

	case State::AI2Strategy:
		// Handle AI 2 strategy selection for AIvsAI mode
		if (m_balancedButton.contains(mousePos)) m_ai2Strategy = Strategy::Balanced;
		else if (m_centerButton.contains(mousePos)) m_ai2Strategy = Strategy::FavorCenter;
		else if (m_edgesButton.contains(mousePos)) m_ai2Strategy = Strategy::FavorEdges;
		else if (m_diagonalButton.contains(mousePos)) m_ai2Strategy = Strategy::FavorDiagonal;
		else if (m_aggressiveButton.contains(mousePos)) m_ai2Strategy = Strategy::Aggressive;
		else if (m_defensiveButton.contains(mousePos)) m_ai2Strategy = Strategy::Defensive;
		else if (m_backFromStrategyButton.contains(mousePos)) {
			m_state = State::AI1Strategy;
			m_subtitleText.setString("Select AI 1 Strategy");
			sf::FloatRect bounds = m_subtitleText.getLocalBounds();
			m_subtitleText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f,
				bounds.position.y + bounds.size.y / 2.0f));
			m_subtitleText.setPosition(sf::Vector2f(m_windowSize.x / 2.0f, m_panel.getPosition().y + 120.0f));
			return true;
		}
		
		// If a strategy was selected (not back button), start game
		if (!m_backFromStrategyButton.contains(mousePos)) {
			m_startGame = true;
			m_state = State::Hidden;
			return true;
		}
		break;

	case State::GameOver:
		if (m_toggleMenuButton.contains(mousePos)) {
			m_menuCollapsed = !m_menuCollapsed;
			m_toggleMenuButton.setText(m_menuCollapsed ? "Show Menu" : "Hide Menu");
			if (m_menuCollapsed) {
				m_toggleMenuButton.setPosition(sf::Vector2f(20.0f, 20.0f));
			} else {
				float panelRight = m_panel.getPosition().x + m_panel.getSize().x;
				float panelTop = m_panel.getPosition().y;
				m_toggleMenuButton.setPosition(sf::Vector2f(panelRight - 200.0f, panelTop + 15.0f));
			}
			return true;
		}
		if (!m_menuCollapsed) {
			if (m_restartButton.contains(mousePos)) {
				m_startGame = true;
				m_winnerMessage.clear();
				m_menuCollapsed = false;
				return true;
			}
			else if (m_mainMenuButton.contains(mousePos)) {
				reset();
				m_menuCollapsed = false;
				return true;
			}
		}
		break;

	case State::Hidden:
		break;
	}

	return false;
}

void Menu::render(sf::RenderWindow& window)
{
	if (m_state == State::Hidden) {
		return;
	}

	switch (m_state) {
	case State::Main:
		window.draw(m_background);
		window.draw(m_panel);
		window.draw(m_titleText);
		window.draw(m_subtitleText);
		m_pvpButton.draw(window);
		m_pvaiButton.draw(window);
		m_aivsaiButton.draw(window);
		m_exitButton.draw(window);
		break;

	case State::Difficulty:
		window.draw(m_background);
		window.draw(m_panel);
		window.draw(m_titleText);
		window.draw(m_subtitleText);
		m_easyButton.draw(window);
		m_mediumButton.draw(window);
		m_hardButton.draw(window);
		m_backButton.draw(window);
		break;

	case State::AIStrategy:
	case State::AI1Strategy:
	case State::AI2Strategy:
		{
			// Temporarily enlarge panel for strategy selection
			sf::Vector2f originalSize = m_panel.getSize();
			sf::Vector2f originalPos = m_panel.getPosition();
			
			float largerHeight = 670.0f;
			m_panel.setSize(sf::Vector2f(originalSize.x, largerHeight));
			
			window.draw(m_background);
			window.draw(m_panel);
			window.draw(m_titleText);
			window.draw(m_subtitleText);
			m_balancedButton.draw(window);
			m_centerButton.draw(window);
			m_edgesButton.draw(window);
			m_diagonalButton.draw(window);
			m_aggressiveButton.draw(window);
			m_defensiveButton.draw(window);
			m_backFromStrategyButton.draw(window);
			
			// Restore original size
			m_panel.setSize(originalSize);
			m_panel.setPosition(originalPos);
		}
		break;

	case State::GameOver:
		if (m_menuCollapsed) {
			m_toggleMenuButton.draw(window);
		} else {
			window.draw(m_background);
			window.draw(m_panel);
			window.draw(m_gameOverText);
			m_restartButton.draw(window);
			m_mainMenuButton.draw(window);
			m_toggleMenuButton.draw(window);
		}
		break;

	case State::Hidden:
		break;
	}
}

void Menu::reset()
{
	m_state = State::Main;
	m_pvpSelected = false;
	m_pvaiSelected = false;
	m_aivsaiSelected = false;
	m_startGame = false;
	m_exitGame = false;
	m_winnerMessage.clear();
	m_subtitleText.setString("Select Game Mode");
	sf::FloatRect bounds = m_subtitleText.getLocalBounds();
	m_subtitleText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f,
		bounds.position.y + bounds.size.y / 2.0f));
	m_subtitleText.setPosition(sf::Vector2f(m_windowSize.x / 2.0f, m_panel.getPosition().y + 120.0f));
}

void Menu::showGameOver(const std::string& winnerText)
{
	m_state = State::GameOver;
	m_winnerMessage = winnerText;
	m_gameOverText.setString(winnerText);
	m_menuCollapsed = false;
	m_toggleMenuButton.setText("Hide Menu");
	
	sf::FloatRect bounds = m_gameOverText.getLocalBounds();
	m_gameOverText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f,
		bounds.position.y + bounds.size.y / 2.0f));
	m_gameOverText.setPosition(sf::Vector2f(m_windowSize.x / 2.0f, m_panel.getPosition().y + 100.0f));
}
