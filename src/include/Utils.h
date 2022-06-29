#ifndef UTILS_H
#define UTILS_H

#include "../Constants.h"
#include "MySet.h"
#include "Pair.h"

enum Direction
{
	UP_LEFT,
	UP_RIGHT,
	DOWN_LEFT,
	DOWN_RIGHT
};

class Utils
{
public:
	Utils();
	virtual ~Utils();

	static int random_number(int l, int u);
	static float frandom_number(float l, float u);

	static bool isCapture(std::array<std::array<char, 8>, 8>& State, Direction dir, PieceType color, Pair pos);

	static std::vector<Pair> legalMovesSquare(std::array<std::array<char, 8>, 8>& State, PieceType color, Pair pos);
	static std::vector<Pair> capturesSquare(std::array<std::array<char, 8>, 8>& State, PieceType color, Pair pos);

	static void capturesPaths(MySet& paths, std::array<std::array<char, 8>, 8>& State, Pair pos, PieceType color, std::vector<Pair>& cur_path);
	static void makeCapture(std::array<std::array<char, 8>, 8>& State, Pair& pos, Pair& cap);

	static void printState(std::array<std::array<char, 8>, 8>& State);
	static std::array<std::array<char, 8>, 8> readState(std::string filename);

	static void makeMove(std::array<std::array<char, 8>, 8>& State, Pair& pos, Pair& mov);
	static std::vector<Pair> capturesPositions(std::array<std::array<char, 8>, 8>& State, PieceType color, PieceType piece);

protected:
private:
};

#endif // UTILS_H
