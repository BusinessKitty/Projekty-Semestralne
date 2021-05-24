#pragma warning(push, 0)
#include <SFML/Graphics.hpp>
#pragma warning(pop)
#include "..\Headers\Text.h"
#include "..\Headers\Drawable.h"
#include "..\Headers\Tile.h"
#include "..\Headers\Blank.h"

Tile::Tile(sf::Vector2f coords_, std::string filename, char letter_,int points_) :Drawable(coords_, filename) {
	setPositionOnBoard();
	status = Status::IN_BAG;
	letter= letter_;
	points = points_;
}
Tile::~Tile() {}

void Tile::setStatus(const Tile::Status& status_) { status = status_; }
Tile::Status Tile::getStatus() { return status; }

char Tile::getLetter() { return letter; }

int Tile::getPoints() { return points; }

void Tile::setPositionOnBoard(const bool& offBoard){
	if (offBoard) positionOnBoard = -1;
	else positionOnBoard = (coords.x - 75)/50 + (coords.y - 75) * 15 / 50;
}
int Tile::getPositionOnBoard(){
	return positionOnBoard;
}