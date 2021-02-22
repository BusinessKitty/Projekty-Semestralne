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

Fruit::Fruit(){}
Fruit::~Fruit(){}
Fruit::Fruit(int x_, int y_, int width_, double scale_, const char* filename_, int points_) : Tile(x_, y_, width_, scale_, filename_),points(points_){}
void Fruit::setPoints(int points_){
	points = points_;
}
int Fruit::getPoints(){
	return points;
}