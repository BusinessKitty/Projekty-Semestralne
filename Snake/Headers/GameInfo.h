#pragma once
#ifndef GAMEINFO_H
#define GAMEINFO_H
class SnakePart;
class Fruit;
class GameInfo {
public:
	int size, width, snakeLength, snakeStartLength, snakeSpeed, currentScore, highScore;
	double scale;
	char* playername;
	char* path;
	int state; // 0-paused, 1-playing, 2-start, 3-lost, 4-won
	int direction; // 0-up, 1-down, 2-left, 3-right

	SnakePart* head;
	Fruit* apple;

	GameInfo();
	GameInfo(int size_, int snakeLength_, int snakeSpeed_, int currentScore_, int highScore_, const char* playername_, const char* path_);
	GameInfo(GameInfo* g);
	~GameInfo();

	void setSize(int size_);
	int highScoreFromFile();
	void updateScoreboard();
	friend ostream& operator<<(ostream& s, const GameInfo& g);
};

#endif