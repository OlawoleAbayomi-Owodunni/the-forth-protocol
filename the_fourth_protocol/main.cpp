/**
 * @file main.cpp
 * @brief Entry point for The Fourth Protocol game
 * @author OA-O and RCH
 * @date November 2025
 * @version 1.0
 *
 * This file contains the main function that initializes and runs
 * The Fourth Protocol game using SFML libraries.
 */

#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 


#include "Game.h"

/**
 * @brief Entry point for all C++ programs
 * 
 * Creates a Game object and starts the main game loop.
 * This function initializes the game and runs until the player exits.
 * 
 * @param argv Array of command line arguments (unused)
 * @return int Exit status (0 for successful completion)
 */
int main(int, char* argv[])
{
	Game game;
	game.run();
	return 0;
}