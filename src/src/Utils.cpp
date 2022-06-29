#include "../include/Utils.h"
#include "../include/Board.h"
#include "../include/Pair.h"

Utils::Utils()
{
	//ctor
}

Utils::~Utils()
{
	//dtor
}

int Utils::random_number(int l, int u)
{
	return rand() % (l - u) + l;
}

float Utils::frandom_number(float l, float u)
{
	return l + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (u - l)));
}

bool Utils::isCapture(std::array<std::array<char, 8>, 8>& State, Direction dir, PieceType color, Pair pos)
{
	switch (dir)
	{
		case UP_LEFT: {
			if (pos.x - 2 < 0 || pos.y - 2 < 0)
			{
				return false;
			}
			return ((color == RED && (State[pos.y - 1][pos.x - 1] == C_BLACK || State[pos.y - 1][pos.x - 1] == C_BLACK_Q))
					   || (color == BLACK && (State[pos.y - 1][pos.x - 1] == C_RED || State[pos.y - 1][pos.x - 1] == C_RED_Q)))
				&& (State[pos.y - 2][pos.x - 2] == C_EMPTY);
		}
		case UP_RIGHT: {
			if (pos.x + 2 > 7 || pos.y - 2 < 0)
			{
				return false;
			}
			return ((color == RED && (State[pos.y - 1][pos.x + 1] == C_BLACK || State[pos.y - 1][pos.x + 1] == C_BLACK_Q))
					   || (color == BLACK && (State[pos.y - 1][pos.x + 1] == C_RED || State[pos.y - 1][pos.x + 1] == C_RED_Q)))
				&& (State[pos.y - 2][pos.x + 2] == C_EMPTY);
		}
		case DOWN_LEFT: {
			if (pos.x - 2 < 0 || pos.y + 2 > 7)
			{
				return false;
			}
			return ((color == RED && (State[pos.y + 1][pos.x - 1] == C_BLACK || State[pos.y + 1][pos.x - 1] == C_BLACK_Q))
					   || (color == BLACK && (State[pos.y + 1][pos.x - 1] == C_RED || State[pos.y + 1][pos.x - 1] == C_RED_Q)))
				&& (State[pos.y + 2][pos.x - 2] == C_EMPTY);
		}
		case DOWN_RIGHT: {
			if (pos.x + 2 > 7 || pos.y + 2 > 7)
			{
				return false;
			}
			return ((color == RED && (State[pos.y + 1][pos.x + 1] == C_BLACK || State[pos.y + 1][pos.x + 1] == C_BLACK_Q))
					   || (color == BLACK && (State[pos.y + 1][pos.x + 1] == C_RED || State[pos.y + 1][pos.x + 1] == C_RED_Q)))
				&& (State[pos.y + 2][pos.x + 2] == C_EMPTY);
		}

		default:
			break;
	}
	return false;
}

std::array<std::array<char, 8>, 8> Utils::readState(std::string filename)
{
	std::array<std::array<char, 8>, 8> State;
	std::ifstream fin(filename);

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			fin >> State[i][j];
		}
	}

	return State;
}

void Utils::printState(std::array<std::array<char, 8>, 8>& State)
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

void Utils::makeMove(std::array<std::array<char, 8>, 8>& State, Pair& pos, Pair& mov)
{
	char piece = State[pos.y][pos.x];
	State[mov.y][mov.x] = piece;
	State[pos.y][pos.x] = C_EMPTY;
}

void Utils::makeCapture(std::array<std::array<char, 8>, 8>& State, Pair& pos, Pair& cap)
{
	char piece = State[pos.y][pos.x];
	int replaceX = (pos.x + cap.x) / 2;
	int replaceY = (pos.y + cap.y) / 2;
	State[pos.y][pos.x] = C_EMPTY;
	State[replaceY][replaceX] = C_EMPTY;
	State[cap.y][cap.x] = piece;
}

void Utils::capturesPaths(MySet& paths, std::array<std::array<char, 8>, 8>& State, Pair pos, PieceType color, std::vector<Pair>& cur_path)
{
	cur_path.push_back(pos);
	/*for (auto& p : cur_path)
	{
		std::cout << p.x << " " << p.y << "\n";
	}
	std::cout << "\n";
	printState(State);
	std::cout << "\n";*/
	paths.add(cur_path);
	std::vector<Pair> captures = capturesSquare(State, color, pos);
	//std::cout << paths.container.size() << "\n";
	if (captures.empty())
	{
		return;
	}
	for (Pair& cap : captures)
	{
		auto State_cpy = State;
		makeCapture(State_cpy, pos, cap);
		auto path = cur_path;
		capturesPaths(paths, State_cpy, cap, color, path);
	}
	paths.remove(cur_path);
}

std::vector<Pair> Utils::capturesSquare(std::array<std::array<char, 8>, 8>& State, PieceType color, Pair pos)
{
	std::vector<Pair> captures;

	switch (color)
	{
		case RED: {
			switch (State[pos.y][pos.x])
			{
				case C_RED: {
					if (isCapture(State, DOWN_LEFT, RED, pos))
					{
						captures.push_back(Pair(pos.x - 2, pos.y + 2));
					}
					if (isCapture(State, DOWN_RIGHT, RED, pos))
					{
						captures.push_back(Pair(pos.x + 2, pos.y + 2));
					}
					break;
				}

				case C_RED_Q: {
					if (isCapture(State, DOWN_LEFT, RED, pos))
					{
						captures.push_back(Pair(pos.x - 2, pos.y + 2));
					}
					if (isCapture(State, DOWN_RIGHT, RED, pos))
					{
						captures.push_back(Pair(pos.x + 2, pos.y + 2));
					}
					if (isCapture(State, UP_LEFT, RED, pos))
					{
						captures.push_back(Pair(pos.x - 2, pos.y - 2));
					}
					if (isCapture(State, UP_RIGHT, RED, pos))
					{
						captures.push_back(Pair(pos.x + 2, pos.y - 2));
					}
					break;
				}

				case C_BLACK: {
					if (isCapture(State, UP_LEFT, BLACK, pos))
					{
						captures.push_back(Pair(pos.x - 2, pos.y - 2));
					}
					if (isCapture(State, UP_RIGHT, BLACK, pos))
					{
						captures.push_back(Pair(pos.x + 2, pos.y - 2));
					}
					break;
				}

				case C_BLACK_Q: {
					if (isCapture(State, DOWN_LEFT, BLACK, pos))
					{
						captures.push_back(Pair(pos.x - 2, pos.y + 2));
					}
					if (isCapture(State, DOWN_RIGHT, BLACK, pos))
					{
						captures.push_back(Pair(pos.x + 2, pos.y + 2));
					}
					if (isCapture(State, UP_LEFT, BLACK, pos))
					{
						captures.push_back(Pair(pos.x - 2, pos.y - 2));
					}
					if (isCapture(State, UP_RIGHT, BLACK, pos))
					{
						captures.push_back(Pair(pos.x + 2, pos.y - 2));
					}
					break;
				}

				default:
					break;
			}
			break;
		}

		case BLACK: {
			switch (State[pos.y][pos.x])
			{
				case C_BLACK: {
					if (isCapture(State, DOWN_LEFT, BLACK, pos))
					{
						captures.push_back(Pair(pos.x - 2, pos.y + 2));
					}
					if (isCapture(State, DOWN_RIGHT, BLACK, pos))
					{
						captures.push_back(Pair(pos.x + 2, pos.y + 2));
					}
					break;
				}

				case C_RED: {
					if (isCapture(State, UP_LEFT, RED, pos))
					{
						captures.push_back(Pair(pos.x - 2, pos.y - 2));
					}
					if (isCapture(State, UP_RIGHT, RED, pos))
					{
						captures.push_back(Pair(pos.x + 2, pos.y - 2));
					}
					break;
				}

				case C_BLACK_Q: {
					if (isCapture(State, DOWN_LEFT, BLACK, pos))
					{
						captures.push_back(Pair(pos.x - 2, pos.y + 2));
					}
					if (isCapture(State, DOWN_RIGHT, BLACK, pos))
					{
						captures.push_back(Pair(pos.x + 2, pos.y + 2));
					}
					if (isCapture(State, UP_LEFT, BLACK, pos))
					{
						captures.push_back(Pair(pos.x - 2, pos.y - 2));
					}
					if (isCapture(State, UP_RIGHT, BLACK, pos))
					{
						captures.push_back(Pair(pos.x + 2, pos.y - 2));
					}
					break;
				}

				case C_RED_Q: {
					if (isCapture(State, DOWN_LEFT, RED, pos))
					{
						captures.push_back(Pair(pos.x - 2, pos.y + 2));
					}
					if (isCapture(State, DOWN_RIGHT, RED, pos))
					{
						captures.push_back(Pair(pos.x + 2, pos.y + 2));
					}
					if (isCapture(State, UP_LEFT, RED, pos))
					{
						captures.push_back(Pair(pos.x - 2, pos.y - 2));
					}
					if (isCapture(State, UP_RIGHT, RED, pos))
					{
						captures.push_back(Pair(pos.x + 2, pos.y - 2));
					}
					break;
				}

				default:
					break;
			}

			break;
		}

		default:
			break;
	}

	return captures;
}

std::vector<Pair> Utils::capturesPositions(std::array<std::array<char, 8>, 8>& State, PieceType color, PieceType piece)
{
	std::vector<Pair> positions;

	for (int i = 0; i <= 6; i += 2)
	{
		for (int j = 1; j <= 7; j += 2)
		{
			switch (piece)
			{
				case RED: {
					if (State[i][j] == C_RED || State[i][j] == C_RED_Q)
					{
						std::vector<Pair> caps = capturesSquare(State, color, Pair(j, i));
						//captures.insert(captures.end(), caps.begin(), caps.end());
						if (caps.size() > 0)
						{
							positions.push_back(Pair(j, i));
						}
					}

					if (State[i + 1][j - 1] == C_RED || State[i + 1][j - 1] == C_RED_Q)
					{
						std::vector<Pair> caps = capturesSquare(State, color, Pair(j - 1, i + 1));
						if (caps.size() > 0)
						{
							positions.push_back(Pair(j - 1, i + 1));
						}
					}

					break;
				}

				case BLACK: {
					if (State[i][j] == C_BLACK || State[i][j] == C_BLACK_Q)
					{
						std::vector<Pair> caps = capturesSquare(State, color, Pair(j, i));
						if (caps.size() > 0)
						{
							positions.push_back(Pair(j, i));
						}
					}

					if (State[i + 1][j - 1] == C_BLACK || State[i + 1][j - 1] == C_BLACK_Q)
					{
						std::vector<Pair> caps = capturesSquare(State, color, Pair(j - 1, i + 1));
						if (caps.size() > 0)
						{
							positions.push_back(Pair(j - 1, i + 1));
						}
					}
					break;
				}

				default:
					break;
			}
		}
	}

	return positions;
}

std::vector<Pair> Utils::legalMovesSquare(std::array<std::array<char, 8>, 8>& State, PieceType color, Pair pos)
{
	std::vector<Pair> moves;

	switch (color)
	{
		case RED: {
			if (State[pos.y][pos.x] == C_RED)
			{
				if (pos.y + 1 < 8 && pos.x - 1 >= 0 && State[pos.y + 1][pos.x - 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x - 1, pos.y + 1));
				}
				if (pos.y + 1 < 8 && pos.x + 1 < 8 && State[pos.y + 1][pos.x + 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x + 1, pos.y + 1));
				}
			}

			if (State[pos.y][pos.x] == C_BLACK)
			{
				if (pos.y - 1 >= 0 && pos.x + 1 < 8 && State[pos.y - 1][pos.x + 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x + 1, pos.y - 1));
				}
				if (pos.x - 1 >= 0 && pos.y - 1 >= 0 && State[pos.y - 1][pos.x - 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x - 1, pos.y - 1));
				}
			}

			if (State[pos.y][pos.x] == C_RED_Q || State[pos.y][pos.x] == C_BLACK_Q)
			{
				if (pos.y - 1 >= 0 && pos.x + 1 < 8 && State[pos.y - 1][pos.x + 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x + 1, pos.y - 1));
				}
				if (pos.x - 1 >= 0 && pos.y - 1 >= 0 && State[pos.y - 1][pos.x - 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x - 1, pos.y - 1));
				}
				if (pos.y + 1 < 8 && pos.x - 1 >= 0 && State[pos.y + 1][pos.x - 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x - 1, pos.y + 1));
				}
				if (pos.y + 1 < 8 && pos.x + 1 < 8 && State[pos.y + 1][pos.x + 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x + 1, pos.y + 1));
				}
			}
			break;
		}

		case BLACK: {
			if (State[pos.y][pos.x] == C_BLACK)
			{
				if (pos.y + 1 < 8 && pos.x - 1 >= 0 && State[pos.y + 1][pos.x - 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x - 1, pos.y + 1));
				}
				if (pos.y + 1 < 8 && pos.x + 1 < 8 && State[pos.y + 1][pos.x + 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x + 1, pos.y + 1));
				}
			}

			if (State[pos.y][pos.x] == C_RED)
			{
				if (pos.y - 1 >= 0 && pos.x + 1 < 8 && State[pos.y - 1][pos.x + 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x + 1, pos.y - 1));
				}
				if (pos.x - 1 >= 0 && pos.y - 1 >= 0 && State[pos.y - 1][pos.x - 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x - 1, pos.y - 1));
				}
			}

			if (State[pos.y][pos.x] == C_BLACK_Q || State[pos.y][pos.x] == C_RED_Q)
			{
				if (pos.y - 1 >= 0 && pos.x + 1 < 8 && State[pos.y - 1][pos.x + 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x + 1, pos.y - 1));
				}
				if (pos.x - 1 >= 0 && pos.y - 1 >= 0 && State[pos.y - 1][pos.x - 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x - 1, pos.y - 1));
				}
				if (pos.y + 1 < 8 && pos.x - 1 >= 0 && State[pos.y + 1][pos.x - 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x - 1, pos.y + 1));
				}
				if (pos.y + 1 < 8 && pos.x + 1 < 8 && State[pos.y + 1][pos.x + 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x + 1, pos.y + 1));
				}
			}
		}

		default:
			break;
	}

	return moves;
}