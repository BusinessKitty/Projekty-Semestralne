#pragma once
#ifndef FRUIT_H
#define FRUIT_H

class Fruit :public Tile {
	int points;
public:

	Fruit();
	Fruit(int x_, int y_, int width_, double scale_, const char* filename_, int points_);
	~Fruit();
	
	int getPoints();
	void setPoints(int points);
};
#endif