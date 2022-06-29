#include "../include/Board.h"
#include "../include/Game.h"
#include "../include/Utils.h"

Board::Board(PieceType playerColor) :
	playerColor(playerColor)
{
	if (!m_Texture.loadFromFile(BOARD_PATH))
	{
		std::cout << "Error loading board texture!\n";
	}
	else
	{
		m_Rectangle.setTexture(&m_Texture);
		m_Rectangle.setPosition(0, 0);
		m_Rectangle.setSize(sf::Vector2f(WIDTH, HEIGHT));
	}
	//initialState();

	initialState();
	toMove = BLACK;
	//State = Utils::readState("content/State.txt");

	//Utils::makeCapture(State, Pair(4, 3), Pair(6, 1));

	/*MySet paths;
	std::vector<Pair> cur_path;
	Utils::capturesPaths(paths, State, Pair(2, 7), RED, cur_path);

	for (auto& a : paths.elements())
	{
		for (auto& p : a)
		{
			std::cout << "(" << p.x << ", " << p.y << "); ";
		}
		std::cout << "\n";
	}*/

	/*std::vector<Pair> l = Utils::capturesSquare(State, BLACK, Pair(4, 4));

	for (auto& p : l)
	{
		std::cout << p.x << " " << p.y << "\n";
	}
*/
	//printState();

	if (!blackTexture.loadFromFile(BLACK_PATH))
	{
		std::cout << "Error loading black texture!\n";
		return;
	}
	if (!redTexture.loadFromFile(RED_PATH))
	{
		std::cout << "Error loading red texture!\n";
		return;
	}
	if (!redQueenTexture.loadFromFile(REDQUEEN_PATH))
	{
		std::cout << "Error loading red queen texture!\n";
		return;
	}
	if (!blackQueenTexture.loadFromFile(BLACKQUEEN_PATH))
	{
		std::cout << "Error loading black queen texture!\n";
		return;
	}

	selectRectangle.setFillColor(sf::Color::Blue);
	selectRectangle.setSize(sf::Vector2f(SQUARE_SIZE + 1, SQUARE_SIZE + 1));
}

void Board::printState()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			std::cout << State[i][j] << " ";
		}
		std::cout << "\n";
	}
}

void Board::draw()
{
	Game::Window.draw(m_Rectangle);

	if (selectedX >= 0 && selectedY >= 0)
	{
		if ((toMove == RED && (State[selectedY][selectedX] == C_RED || State[selectedY][selectedX] == C_RED_Q))
			|| (toMove == BLACK && (State[selectedY][selectedX] == C_BLACK || State[selectedY][selectedX] == C_BLACK_Q)))
		{
			if (positionsCaptures.empty()
				|| (std::find(positionsCaptures.begin(), positionsCaptures.end(), Pair(selectedX, selectedY)) != positionsCaptures.end()))
			{
				Game::Window.draw(selectRectangle);
			}
			drawLegalMovesSquare();
		}
	}

	drawState();
}

void Board::update()
{
	if (Game::isMousePressed)
	{
		positionsCaptures = Utils::capturesPositions(State, playerColor, toMove);
		int prevX = selectedX;
		int prevY = selectedY;
		selectedX = floor(Game::mouseEvent.mouseButton.x / SQUARE_SIZE);
		selectedY = floor(Game::mouseEvent.mouseButton.y / SQUARE_SIZE);
		selectRectangle.setPosition(selectedX * (SQUARE_SIZE + 0.85f), selectedY * (SQUARE_SIZE + 0.85f));
		if (positionsCaptures.empty())
		{
			if (std::find(legalMovesSquare.begin(), legalMovesSquare.end(), Pair(selectedX, selectedY)) != legalMovesSquare.end())
			{
				Pair prev = Pair(prevX, prevY);
				Pair selected = Pair(selectedX, selectedY);
				Utils::makeMove(State, prev, selected);

				promote();

				switch (toMove)
				{
					case RED: {
						toMove = BLACK;
						break;
					}

					case BLACK: {
						toMove = RED;
						break;
					}

					default:
						break;
				}
			}
			switch (toMove)
			{
				case BLACK: {
					if (State[selectedY][selectedX] != C_RED && State[selectedY][selectedX] != C_RED_Q)
					{
						legalMovesSquare = Utils::legalMovesSquare(State, playerColor, Pair(selectedX, selectedY));
					}
					break;
				}

				case RED: {
					if (State[selectedY][selectedX] != C_BLACK && State[selectedY][selectedX] != C_BLACK_Q)
					{
						legalMovesSquare = Utils::legalMovesSquare(State, playerColor, Pair(selectedX, selectedY));
					}
					break;
				}

				default:
					break;
			}
		}
		else
		{
			if (std::find(legalMovesSquare.begin(), legalMovesSquare.end(), Pair(selectedX, selectedY)) != legalMovesSquare.end())
			{
				Pair prev = Pair(prevX, prevY);
				Pair selected = Pair(selectedX, selectedY);
				Utils::makeCapture(State, prev, selected);

				promote();

				positionsCaptures = Utils::capturesPositions(State, playerColor, toMove);
				if (positionsCaptures.empty())
				{
					switch (toMove)
					{
						case RED: {
							toMove = BLACK;
							break;
						}

						case BLACK: {
							toMove = RED;
							break;
						}

						default:
							break;
					}
				}
			}
			switch (toMove)
			{
				case BLACK: {
					if (State[selectedY][selectedX] != C_RED && State[selectedY][selectedX] != C_RED_Q
						&& std::find(positionsCaptures.begin(), positionsCaptures.end(), Pair(selectedX, selectedY)) != positionsCaptures.end())
					{
						std::vector<Pair> captures = Utils::capturesSquare(State, playerColor, Pair(selectedX, selectedY));
						legalMovesSquare.clear();
						legalMovesSquare.insert(legalMovesSquare.end(), captures.begin(), captures.end());
					}
					else
					{
						legalMovesSquare.clear();
					}
					break;
				}

				case RED: {
					if (State[selectedY][selectedX] != C_BLACK && State[selectedY][selectedX] != C_BLACK_Q
						&& std::find(positionsCaptures.begin(), positionsCaptures.end(), Pair(selectedX, selectedY)) != positionsCaptures.end())
					{
						std::vector<Pair> captures = Utils::capturesSquare(State, playerColor, Pair(selectedX, selectedY));
						legalMovesSquare.clear();
						legalMovesSquare.insert(legalMovesSquare.end(), captures.begin(), captures.end());
					}
					else
					{
						legalMovesSquare.clear();
					}
					break;
				}

				default:
					break;
			}
		}
	}
}

void Board::promote()
{
	switch (playerColor)
	{
		case BLACK: {
			switch (toMove)
			{
				case BLACK: {
					if (selectedY == 7)
					{
						State[selectedY][selectedX] = C_BLACK_Q;
					}
					break;
				}

				case RED: {
					if (selectedY == 0)
					{
						State[selectedY][selectedX] = C_RED_Q;
					}
				}

				default:
					break;
			}
			break;
		}

		case RED: {
			switch (toMove)
			{
				case BLACK: {
					if (selectedY == 0)
					{
						State[selectedY][selectedX] = C_BLACK_Q;
					}
					break;
				}

				case RED: {
					if (selectedY == 7)
					{
						State[selectedY][selectedX] = C_RED_Q;
					}
				}

				default:
					break;
			}
			break;
		}

		default:
			break;
	}
}

void Board::emptyState()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			State[i][j] = C_EMPTY;
		}
	}
}

void Board::initialState()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			State[i][j] = C_EMPTY;
		}
	}
	if (playerColor == BLACK)
	{
		for (int j = 1; j <= 7; j += 2)
		{
			State[0][j] = C_BLACK;
			State[1][j - 1] = C_BLACK;
			State[2][j] = C_BLACK;

			State[5][j - 1] = C_RED;
			State[6][j] = C_RED;
			State[7][j - 1] = C_RED;
		}
	}
	if (playerColor == RED)
	{
		for (int j = 1; j <= 7; j += 2)
		{
			State[0][j] = C_RED;
			State[1][j - 1] = C_RED;
			State[2][j] = C_RED;

			State[5][j - 1] = C_BLACK;
			State[6][j] = C_BLACK;
			State[7][j - 1] = C_BLACK;
		}
	}
}

void Board::drawPiece(sf::Texture& texture, int px, int py)
{
	sf::RectangleShape rectangle;
	rectangle.setTexture(&texture);
	rectangle.setPosition(px * SQUARE_SIZE + PIECE_OFFSET - 2, py * SQUARE_SIZE + PIECE_OFFSET - 2);
	rectangle.setSize(sf::Vector2f(PIECE_SIZE, PIECE_SIZE));
	Game::Window.draw(rectangle);
}

void Board::drawLegalMovesSquare()
{
	for (Pair& pair : legalMovesSquare)
	{
		sf::RectangleShape rect;
		rect.setFillColor(sf::Color::Cyan);
		rect.setSize(sf::Vector2f(SQUARE_SIZE + 1, SQUARE_SIZE + 1));
		rect.setPosition(pair.x * (SQUARE_SIZE + 0.85f), pair.y * (SQUARE_SIZE + 0.85f));
		Game::Window.draw(rect);
	}
}

void Board::drawState()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			switch (State[i][j])
			{
				case C_RED: {
					drawPiece(redTexture, j, i);
					break;
				}
				case C_BLACK: {
					drawPiece(blackTexture, j, i);
					break;
				}
				case C_BLACK_Q: {
					drawPiece(blackQueenTexture, j, i);
					break;
				}
				case C_RED_Q: {
					drawPiece(redQueenTexture, j, i);
					break;
				}

				default: {
					break;
				}
			}
		}
	}
}