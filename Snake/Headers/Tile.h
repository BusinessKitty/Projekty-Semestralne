#pragma once
#ifndef TILE_H
#define TILE_H

class Tile {
	int x, y;
	Texture texture;
	Sprite sprite;
public:
	bool isEmpty; //1-empty 0-snake/apple

	Tile();
	Tile(int x_, int y_,int width, double scale,const char* filename);
	~Tile();

	void setX(int x_);
	void setY(int y_);
	int getX();
	int getY();
	void loadSprite(double scale,const char* filename);
	Sprite* getSprite();
	void updatePosition(int width);

	bool operator==(const Tile& tile);
};

#endif