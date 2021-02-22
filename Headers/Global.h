#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H

char* date();
GameInfo* gameSetup(int argc, char* argv[]);
GameInfo* gameSetup(char* path);
void writeToFile(GameInfo* g);
Tile** boardSetup(GameInfo* game);
void Tick(GameInfo* game,Tile** board);
void updateBoardStatus(SnakePart*& head, Tile** board);
void deleteSnake(SnakePart*& head);
#endif