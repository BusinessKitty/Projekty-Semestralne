#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <ctime>
using namespace std;
using namespace sf;

#include "..\Headers\GameInfo.h"
#include "..\Headers\Tile.h"
#include "..\Headers\SnakePart.h"
#include "..\Headers\Fruit.h"
#include "..\Headers\Global.h"

GameInfo::GameInfo() :snakeLength(4), snakeStartLength(4), snakeSpeed(2), currentScore(0),
playername(new char[20]), path(new char[2]), head(nullptr),
state(2), direction(0) {
    setSize(16);

    char* playername_ = new char[14];
    strcpy_s(playername_, 8, "Player_");
    strcat(playername_, date());
    strcpy_s(playername, strlen(playername_)+1, playername_);
    strcpy_s(path, 2, "-");
	highScore = highScoreFromFile();

	apple = new Fruit(rand() % (size - 1), rand() % (size - 1), width, scale, "Resources/fruit.png", snakeSpeed);
}
GameInfo::GameInfo(int size_, int snakeLength_, int snakeSpeed_, int currentScore_, int highScore_, const char* playername_, const char* path_)
    :snakeLength(snakeLength_), snakeStartLength(snakeLength_), snakeSpeed(snakeSpeed_), currentScore(currentScore_),
    playername(new char[strlen(playername_) + 1]), path(new char[strlen(path_) + 1]), head(nullptr),
    state(0), direction(0) {

    setSize(size_);
    strcpy_s(playername, strlen(playername_) + 1, playername_);
    strcpy_s(path, strlen(path_) + 1, path_);
	highScore = highScoreFromFile();
}
GameInfo::GameInfo(GameInfo* g) :snakeLength(g->snakeStartLength), snakeStartLength(g->snakeStartLength), snakeSpeed(g->snakeSpeed), currentScore(0), highScore(g->highScore),
playername(new char[strlen(g->playername) + 1]), path(new char[strlen(g->path) + 1]), head(nullptr),
state(2), direction(0) {
    setSize(g->size);
    strcpy_s(playername, strlen(g->playername) + 1, g->playername);
    strcpy_s(path, strlen(g->path) + 1, g->path);

	apple = new Fruit(rand() % (size - 1), rand() % (size - 1), width, scale, "Resources/fruit.png", snakeSpeed);
}
GameInfo::~GameInfo(){
	if(playername) delete[] playername;
	if(path) delete[] path;
	if(apple) delete apple;
	apple = nullptr;
	deleteSnake(head);
}

void GameInfo::setSize(int size_) {
    size = size_;
    width = (576 / size_);
    scale = ((double)8 / (double)size_);
}

int GameInfo::highScoreFromFile(){
	int score;
	FILE* file = fopen("Scoreboard.txt", "r");
	if (file) {
		//wczytaj plik scoreboard.txt do char* file_content:
		fseek(file, 0, SEEK_END);
		int length = ftell(file);
		fseek(file, 0, SEEK_SET);
		char* file_content = new char[length];
		sprintf(file_content, "");
		char c;
		while (1) {
			c = fgetc(file);
			if (feof(file)) {
				break;
			}
			strncat(file_content, &c, 1);
		}
		fclose(file);
		file = nullptr;

		//znajdź char* playername w char* file_content:
		char* temp = new char[2 + strlen(playername)];
		strcpy_s(temp, strlen(playername) + 1, playername);
		strncat(temp, " ", 2);
		char* ptr = strstr(file_content, temp);
		//jesli znalazłeś to zapisz jego highscore do temp:
		if (ptr) {
			delete[] temp;
			temp = new char[5];
			for (int i = 0; i < 4; i++) {
				temp[i] = ptr[strlen(playername) + 1 + i];
			}
			temp[4] = '\0';
			//zwróć wynik
			score = atoi(temp);
		}
		//jeśli nie, zwróć 0
		else {
			score = 0;
		}
		delete[] temp;
		delete[] file_content;
		return score;
	}
	else {
		return 0;
	}
}

void GameInfo::updateScoreboard() {
	FILE* file = fopen("Scoreboard.txt", "r");
	if (file) {
		//wczytaj plik scoreboard.txt do char* file_content:
		fseek(file, 0, SEEK_END);
		int length = ftell(file);
		fseek(file, 0, SEEK_SET);
		char* file_content = new char[length];
		sprintf(file_content, "");
		char c;
		while (1) {
			c = fgetc(file);
			if (feof(file)) {
				break;
			}
			strncat(file_content, &c, 1);
		}
		fclose(file);
		file = nullptr;

		//znajdź char* playername w char* file_content:
		char* temp = new char[2 + strlen(playername)];
		strcpy_s(temp, strlen(playername) + 1, playername);
		strncat(temp, " ", 2);
		char* ptr = strstr(file_content, temp);
		//jesli znalazłeś to zapisz jego highscore do temp:
		if (ptr) {
			delete[] temp;
			temp = new char[5];
			for (int i = 0; i < 4; i++) {
				temp[i] = ptr[strlen(playername) + 1 + i];
			}
			temp[4] = '\0';
			//jeśli aktualny highscore jest większy niż ten z pliku, aktualizuj plik
			if (atoi(temp) < highScore) {
				ptr[strlen(playername) + 4] = highScore % 10 + 48;
				ptr[strlen(playername) + 3] = (highScore / 10)   % 10 + 48;
				ptr[strlen(playername) + 2] = (highScore / 100)  % 10 + 48;
				ptr[strlen(playername) + 1] = (highScore / 1000) % 10 + 48;

				file = fopen("Scoreboard.txt", "w");
				fprintf(file, "%s", file_content);
				fclose(file);
				file = nullptr;
			}
		}
		//jeśli nie znalazłeś to dopisz '\nplayername highScore' do pliku
		else {
			delete[] temp;
			temp = new char[strlen(file_content)+strlen(playername)+7];
			strcpy_s(temp, strlen(file_content) + 1, file_content);
			strncat(temp, "\n", 2); 
			strncat(temp, playername, strlen(playername)+1); 
			strncat(temp, " ", 2);
			char* c = new char[2];
			c[0] = (highScore / 1000) % 10 + 48;
			strncat(temp, c, 1);
			c[0] = (highScore / 100)  % 10 + 48;
			strncat(temp, c, 1);
			c[0] = (highScore / 10)   % 10 + 48;
			strncat(temp, c, 1);
			c[0] = highScore % 10 + 48;
			strncat(temp, c, 1);
			delete[] c;

			file = fopen("Scoreboard.txt", "w");
			fprintf(file, "%s", temp);
			fclose(file);
			file = nullptr;
		}
		delete[] temp;
		delete[] file_content;
	}
}