#pragma warning(push, 0)
#include <SFML/Graphics.hpp>
#pragma warning(pop)
#include "..\Headers\Text.h"
#include "..\Headers\Drawable.h"
#include "..\Headers\Slot.h"

Slot::Slot(sf::Vector2f coords_, std::string filename, std::pair<int, Slot::MultiplierType> multiplier_)
	: Drawable(coords_, filename), taken(0)
{
	multiplier.first = multiplier_.first;
	multiplier.second = multiplier_.second;
	positionOnBoard = (coords.x - 75) / 50 + (coords.y - 75) * 15 / 50;
}
Slot::Slot() : taken(0), positionOnBoard(-1) {};
Slot::~Slot() {};

std::pair<int, Slot::MultiplierType> Slot::getMultiplier() { return multiplier; }

int Slot::getPositionOnBoard() {
	return positionOnBoard;
}
void Slot::takeSlot() {
	taken = 1;
}
void Slot::leaveSlot() {
	taken = 0;
}
bool Slot::isTaken() {
	return taken;
}