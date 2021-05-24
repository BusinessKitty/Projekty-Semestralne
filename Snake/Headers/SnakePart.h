#pragma once
#ifndef SNAKEPART_H
#define SNAKEPART_H

class SnakePart : public Tile {
public:
	SnakePart* next;
	SnakePart* prev;
	SnakePart();
	SnakePart(int x_, int y_,int width_, double scale,const char* filename_, SnakePart* next_, SnakePart* prev_);
	~SnakePart();

	void updateSnake(int width);
	static void draw(SnakePart*& head, RenderWindow* window,double scale);
	static void move(SnakePart*& head);
	static void grow(SnakePart*& head, GameInfo* game);
	static void checkCollision(SnakePart*& head, GameInfo* game);
};
#endif