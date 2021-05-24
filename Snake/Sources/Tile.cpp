#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
using namespace std;
using namespace sf;

#include "..\Headers\GameInfo.h"
#include "..\Headers\Tile.h"
#include "..\Headers\SnakePart.h"
#include "..\Headers\Fruit.h"
#include "..\Headers\Global.h"

Tile::Tile() {}
Tile::Tile(int x_, int y_, int width,double scale, const char* filename) :x(x_),y(y_){
	loadSprite(scale, filename);
	updatePosition(width);
}
Tile::~Tile() {}

void Tile::setX(int x_) {
	x = x_;
}
void Tile::setY(int y_) {
	y = y_;
}
int Tile::getX() {
	return x;
}
int Tile::getY() {
	return y;
}
void Tile::loadSprite(double scale, const char* filename) {
	texture.loadFromFile(filename);
	sprite.setTexture(texture);
	sprite.setScale(scale, scale);
}
void Tile::updatePosition(int width) {
	sprite.setPosition(72 + x*width, 72 + y*width);
}
Sprite* Tile::getSprite() {
	return &sprite;
}

bool Tile::operator==(const Tile& tile) {
	if (x == tile.x && y == tile.y) return 1;
	return 0;
}