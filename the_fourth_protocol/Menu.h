/**
 * @file Menu.h
 * @brief Menu system for The Fourth Protocol
 * @author RCH
 * @date December 2025
 * @version 1.0
 *
 * This file contains the Menu class which manages the main menu,
 * difficulty selection, and game mode options with interactive buttons.
 */

#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <functional>
#include "AI.h"

/// @brief Difficulty levels for AI opponent
enum class Difficulty {
	Easy,		///< Easy difficulty - AI searches 2 moves ahead
	Medium,		///< Medium difficulty - AI searches 3 moves ahead
	Hard		///< Hard difficulty - AI searches 3 moves ahead but grid is 7 x 7
};

/// @brief Interactive button class with hover effects
class MenuButton {
public:
	/// @brief Constructor for menu button
	/// @param position Position of the button
	/// @param size Size of the button
	/// @param text Text to display on button
	/// @param font Font to use for text
	/// @param normalColor Button color when not hovered
	/// @param hoverColor Button color when hovered
	MenuButton(const sf::Vector2f& position, const sf::Vector2f& size, 
		const std::string& text, const sf::Font& font,
		const sf::Color& normalColor = sf::Color(70, 130, 180),
		const sf::Color& hoverColor = sf::Color(100, 149, 237));

	/// @brief Update button hover state based on mouse position
	/// @param mousePos Current mouse position
	void update(const sf::Vector2f& mousePos);

	/// @brief Draw the button to the render window
	/// @param window Window to draw to
	void draw(sf::RenderWindow& window);

	/// @brief Check if button contains a point
	/// @param point Point to check
	/// @return True if point is inside button bounds
	bool contains(const sf::Vector2f& point) const;

	/// @brief Check if button is currently being hovered
	/// @return True if mouse is over button
	bool isHovered() const { return m_isHovered; }

	/// @brief Set button enabled/disabled state
	/// @param enabled True to enable, false to disable
	void setEnabled(bool enabled);

	/// @brief Check if button is enabled
	/// @return True if button is enabled
	bool isEnabled() const { return m_enabled; }

	/// @brief Set button text
	/// @param text New text for button
	void setText(const std::string& text);

	/// @brief Set button position
	/// @param position New position for button
	void setPosition(const sf::Vector2f& position);

private:
	const sf::Font& m_font;			///< Reference to font
	sf::RectangleShape m_shape;		///< Button background shape
	sf::Text m_text{ m_font };		///< Button text label
	sf::Color m_normalColor;		///< Color when not hovered
	sf::Color m_hoverColor;			///< Color when hovered
	sf::Color m_disabledColor;		///< Color when disabled
	bool m_isHovered;				///< Current hover state
	bool m_enabled;					///< Whether button is enabled
};

/// @brief Main menu system for the game
class Menu {
public:
	/// @brief Menu state enumeration
	enum class State {
		Main,			///< Main menu showing game mode selection
		Difficulty,		///< Difficulty selection for AI mode
		AIStrategy,		///< AI strategy selection (for PvAI mode)
		AI1Strategy,	///< AI 1 strategy selection (for AIvsAI mode)
		AI2Strategy,	///< AI 2 strategy selection (for AIvsAI mode)
		GameOver,		///< Game over screen with restart/menu options
		Hidden			///< Menu is hidden, game is running
	};

	/// @brief Constructor
	/// @param font Font to use for all menu text
	/// @param windowSize Size of the game window
	Menu(const sf::Font& font, const sf::Vector2f& windowSize);

	/// @brief Update menu state based on mouse position
	/// @param mousePos Current mouse position
	void update(const sf::Vector2f& mousePos);

	/// @brief Handle mouse click events
	/// @param mousePos Position where mouse was clicked
	/// @return True if a menu action was triggered
	bool handleClick(const sf::Vector2f& mousePos);

	/// @brief Draw the menu
	/// @param window Window to draw to
	void render(sf::RenderWindow& window);

	/// @brief Get current menu state
	/// @return Current state
	State getState() const { return m_state; }

	/// @brief Set menu state
	/// @param state New state
	void setState(State state) { m_state = state; }

	/// @brief Check if PvP mode was selected
	/// @return True if player vs player mode selected
	bool isPvPSelected() const { return m_pvpSelected; }

	/// @brief Check if PvAI mode was selected
	/// @return True if player vs AI mode selected
	bool isPvAISelected() const { return m_pvaiSelected; }

	/// @brief Check if AI vs AI mode was selected
	/// @return True if AI vs AI mode selected
	bool isAIvsAISelected() const { return m_aivsaiSelected; }

	/// @brief Get selected difficulty level
	/// @return Selected difficulty
	Difficulty getDifficulty() const { return m_selectedDifficulty; }

	/// @brief Get selected AI strategy (for PvAI mode)
	/// @return Selected AI strategy
	Strategy getAIStrategy() const { return m_aiStrategy; }

	/// @brief Get selected AI 1 strategy (for AIvsAI mode)
	/// @return Selected AI 1 strategy
	Strategy getAI1Strategy() const { return m_ai1Strategy; }

	/// @brief Get selected AI 2 strategy (for AIvsAI mode)
	/// @return Selected AI 2 strategy
	Strategy getAI2Strategy() const { return m_ai2Strategy; }

	/// @brief Reset menu to initial state
	void reset();

	/// @brief Check if game should start
	/// @return True if start game was selected
	bool shouldStartGame() const { return m_startGame; }

	/// @brief Check if game should exit
	/// @return True if exit was selected
	bool shouldExitGame() const { return m_exitGame; }

	/// @brief Clear start game flag
	void clearStartFlag() { m_startGame = false; }

	/// @brief Show game over screen
	/// @param winnerText Text to display for winner
	void showGameOver(const std::string& winnerText);

private:
	/// @brief Initialize main menu buttons
	void initMainMenu();

	/// @brief Initialize difficulty selection buttons
	void initDifficultyMenu();

	/// @brief Initialize strategy selection buttons
	void initStrategyMenu();

	/// @brief Initialize game over menu
	void initGameOverMenu();

	const sf::Font& m_font;				///< Reference to game font
	sf::Vector2f m_windowSize;			///< Window dimensions
	State m_state;						///< Current menu state

	// Menu backgrounds
	sf::RectangleShape m_background;	///< Menu background overlay
	sf::RectangleShape m_panel;			///< Menu panel
	sf::Text m_titleText{ m_font };		///< Menu title
	sf::Text m_subtitleText{ m_font };	///< Menu subtitle

	// Main menu buttons
	std::vector<MenuButton> m_mainButtons;		///< Main menu buttons
	MenuButton m_pvpButton;				///< Player vs Player button
	MenuButton m_pvaiButton;			///< Player vs AI button
	MenuButton m_aivsaiButton;			///< AI vs AI button
	MenuButton m_exitButton;			///< Exit button

	// Difficulty menu buttons
	std::vector<MenuButton> m_difficultyButtons;	///< Difficulty selection buttons
	MenuButton m_easyButton;			///< Easy difficulty button
	MenuButton m_mediumButton;			///< Medium difficulty button
	MenuButton m_hardButton;			///< Hard difficulty button
	MenuButton m_backButton;			///< Back to main menu button

	// Strategy menu buttons
	MenuButton m_balancedButton;		///< Balanced strategy button
	MenuButton m_centerButton;			///< Favor center strategy button
	MenuButton m_edgesButton;			///< Favor edges strategy button
	MenuButton m_diagonalButton;		///< Favor diagonal strategy button
	MenuButton m_aggressiveButton;		///< Aggressive strategy button
	MenuButton m_defensiveButton;		///< Defensive strategy button
	MenuButton m_backFromStrategyButton;///< Back button from strategy menu

	// Game over menu
	MenuButton m_restartButton;			///< Restart game button
	MenuButton m_mainMenuButton;		///< Return to main menu button
	MenuButton m_toggleMenuButton;		///< Toggle menu visibility button
	sf::Text m_gameOverText{ m_font };	///< Game over message
	bool m_menuCollapsed;				///< Whether game over menu is collapsed

	// State flags
	bool m_pvpSelected;					///< PvP mode selected flag
	bool m_pvaiSelected;				///< PvAI mode selected flag
	bool m_aivsaiSelected;				///< AI vs AI mode selected flag
	Difficulty m_selectedDifficulty;	///< Selected difficulty level
	Strategy m_aiStrategy;				///< Selected AI strategy (PvAI)
	Strategy m_ai1Strategy;				///< Selected AI 1 strategy (AIvsAI)
	Strategy m_ai2Strategy;				///< Selected AI 2 strategy (AIvsAI)
	bool m_startGame;					///< Start game flag
	bool m_exitGame;					///< Exit game flag
	std::string m_winnerMessage;		///< Winner message for game over
};
