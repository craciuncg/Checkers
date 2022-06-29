#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <math.h>
#include <set>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock,
	std::chrono::duration<double>>;

// Epsilon for precision range
const float epsilon = 2.5f;

/* Class Declarations */

class Game;

class UIElement;
class UIButton;

class MenuState;
class PlayState;
class Board;

enum PieceChar : char
{
	C_RED = 'r',
	C_RED_Q = 'R',
	C_BLACK = 'b',
	C_BLACK_Q = 'B',
	C_EMPTY = '.'
};

enum PieceType
{
	RED,
	BLACK,
	RED_QUEEN,
	BLACK_QUEEN
};

/* Board Settings */

static int screenWIDTH = sf::VideoMode::getDesktopMode().width;
static int screenHEIGHT = sf::VideoMode::getDesktopMode().height;

const float percentX = 0.45f;
const float percentY = 0.75f;

const int WIDTH = percentX * screenWIDTH;
const int HEIGHT = WIDTH;
const std::string TITLE = "Checkers";

const std::string BOARD_PATH = "content/board.jpg";

/* Piece Settings */

const float SQUARE_SIZE = WIDTH / 8;
const int PIECE_OFFSET = 10;
const int PIECE_SIZE = WIDTH / 8 - PIECE_OFFSET;

const std::string BLACK_PATH = "content/black.png";
const std::string BLACKQUEEN_PATH = "content/black_queen.png";
const std::string RED_PATH = "content/red.png";
const std::string REDQUEEN_PATH = "content/red_queen.png";

/* UI Settings */

const std::string BUTTON_PATH = "content/button.png";

const float percentButtonX = 0.2f;
const float percentButtonY = 0.05f;

const size_t BUTTON_WIDTH = percentButtonX * screenWIDTH;
const size_t BUTTON_HEIGHT = percentButtonY * screenHEIGHT;

#endif // CONSTANTS_H_INCLUDED
