#pragma once

#include "../Constants.h"
#include "Entity.h"
#include "Pair.h"

class Board : public Entity
{
public:
	std::array<std::array<char, 8>, 8> State;

	PieceType playerColor;
	PieceType toMove;

	int selectedX = -1;
	int selectedY = -1;

	Board(PieceType playerColor);
	virtual ~Board() = default;

	virtual void draw();
	virtual void update();

	void printState();

private:
	sf::Texture m_Texture;
	sf::RectangleShape m_Rectangle;

	sf::Texture blackTexture;
	sf::Texture redTexture;
	sf::Texture blackQueenTexture;
	sf::Texture redQueenTexture;

	sf::RectangleShape selectRectangle;

	bool moved = false;

	std::vector<Pair> legalMovesSquare;
	std::vector<Pair> positionsCaptures;

	void initialState();
	void drawState();
	void drawPiece(sf::Texture& texture, int px, int py);
	void drawLegalMovesSquare();
	void emptyState();
	void promote();
};