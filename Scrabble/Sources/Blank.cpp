#pragma warning(push, 0)
#include <SFML/Graphics.hpp>
#pragma warning(pop)

#include "..\Headers\Text.h"
#include "..\Headers\Drawable.h"
#include "..\Headers\Tile.h"
#include "..\Headers\Blank.h"

Blank::Blank(sf::Vector2f coords_, std::string filename_, char letter_, int points_) :Tile(coords_, filename_, letter_, points_) {
	picked = '_';
}
Blank::~Blank(){}

void Blank::setPicked(const char& picked_) {
	picked = picked_;
}
char Blank::getPicked() {
	return picked;
}