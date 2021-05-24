#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <fstream>
using namespace std;
using namespace sf;

#include "..\Headers\GameInfo.h"
#include "..\Headers\Tile.h"
#include "..\Headers\SnakePart.h"
#include "..\Headers\Fruit.h"
#include "..\Headers\Global.h"

char* date() {
	char* date = new char[9];
	date[0] = '\0';
	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);

	char* Day = new char[3];
	sprintf_s(Day, 3, "%d", newtime.tm_mday);
	if (Day[1] == '\0') {
		Day[1] = Day[0];
		Day[0] = '0';
		Day[2] = '\0';
	}
	char* Month = new char[3];
	sprintf_s(Month, 3, "%d", 1 + newtime.tm_mon);
	if (Month[1] == '\0') {
		Month[1] = Month[0];
		Month[0] = '0';
		Month[2] = '\0';
	}
	char* Year = new char[5];
	sprintf_s(Year, 5, "%d", 1900 + newtime.tm_year);
	strcat(date, Day);
	strcat(date, Month);
	strcat(date, Year);
	delete[] Day; delete[] Month; delete[] Year;
	return date;
}

GameInfo* gameSetup(int argc, char* argv[]) {
	GameInfo* g = new GameInfo();
	for (int i = 1; i < argc; i++) {
		//-help
		if (!strcmp(argv[i], "-help")) {
			cout << "-size {8;16;32}    Okreslenie wielkosci planszy" << endl;
			cout << "-speed {1,2,3}     Okreslenie predkosci weza" << endl;
			cout << "-length <2,8>      Okreslenie dlugosci weza" << endl;
			cout << "-player nazwa      Ustawienie nazwy gracza na nazwa" << endl;
			cout << "-game nazwa        Jesli przelacznik wystapi sam: wczytanie gry o podanej nazwie" << endl;
			cout << "                   Jesli przelacznik wystapi razem z innymi: stworzenie/nadpisanie gry o podanych parametrach" << endl;
			return nullptr;
		}
		//-size {8,16,32}
		if (!strcmp(argv[i], "-size") && argv[i + 1]) {
			if (!strcmp(argv[i + 1], "8"))		 g->setSize(8);
			else if (!strcmp(argv[i + 1], "16")) g->setSize(16);
			else if (!strcmp(argv[i + 1], "32")) g->setSize(32);
			else {
				cout << "podano zly parametr przelacznika -size, ustawiono rozmiar planszy na 16x16" << endl;
			}
		}
		//-length <2,8>
		if (!strcmp(argv[i], "-length") && argv[i + 1]) {
			int len = atoi(argv[i + 1]);
			if (len >= 2 && len <= 8) {
				g->snakeLength = len;
				g->snakeStartLength = len;
			}
			else {
				cout << "podano zly parametr przelacznika -length, ustawiono poczatkowa dlugosc weza na 4" << endl;
			}
		}
		//-speed {1,2,3}
		if (!strcmp(argv[i], "-speed") && argv[i + 1]) {
			int sp = atoi(argv[i + 1]);
			if (sp == 1)		g->snakeSpeed = 1;
			else if (sp == 2)	g->snakeSpeed = 2;
			else if (sp == 3)	g->snakeSpeed = 3;
			else {
				cout << "podano zly parametr przelacznika -speed, ustawiono predkosc gry na 2" << endl;
			}
		}
		//-player <nazwa_gracza>
		if (!strcmp(argv[i], "-player")) {
			if (argv[i + 1] && argv[i + 1][0] != '-' && argv[i + 1][0] != '\\') {
				delete[] g->playername;
				g->playername = nullptr;
				g->playername = new char[16];
				strcpy_s(g->playername, strlen(argv[i + 1]) + 1, argv[i + 1]);
				g->playername[15] = '\0';
				g->highScore = g->highScoreFromFile();
			}
			else {
				cout << "podano zly parametr przelacznika -player, ustawiono nazwe gracza na " << g->playername << endl;
			}
		}
		//-game <nazwa_gry>
		if (!strcmp(argv[i], "-game")) {
			if (argv[i + 1] && argv[i + 1][0] != '-') {
				delete[] g->path;
				g->path = nullptr;
				g->path = new char[40];
				strcpy_s(g->path, 17, "Resources/Saves/");
				strncat(g->path, argv[i+1],14);
				//jeśli nazwa podana przez użytkownika nie zawierała rozszerzenia .bin, dodaj je:
				if (!(g->path[15 + strlen(argv[i+1])] == 'n' && g->path[14 + strlen(argv[i+1])] == 'i' && g->path[13 + strlen(argv[i+1])] == 'b' && g->path[12 + strlen(argv[i+1])] == '.')) strcat(g->path, ".bin");
			}
			else {
				cout << "podano zly parametr przelacznika -game, gra nie zostanie zapisana!" << endl;
			}
		}
	}
	//wąż:
	g->head = new SnakePart((g->size) - 1, (g->size) - 1, g->width, g->scale, "Resources/head_u.png", nullptr, nullptr);
	auto w = g->head;
	for (int i = 0; i < g->snakeLength - 1; i++) {
		w->next = new SnakePart(w->getX(), w->getY(), g->width, g->scale, "Resources/transparent.png", nullptr, w);
		w = w->next;
	}
	//jabłko
	g->apple = new Fruit(rand() % (g->size - 1), rand() % (g->size - 1), g->width, g->scale, "Resources/fruit.png", g->snakeSpeed);
	return g;
}

GameInfo* gameSetup(char* path) {
	GameInfo* g = new GameInfo();
	fstream in;
	in.open(path, ios::in | ios::binary);
	if (in) {
		in.read((char*)g, sizeof(GameInfo));
		
		int len = 0;
		in.read((char*)&len, sizeof(len));
		//if (g->playername)delete[] g->playername;
		g->playername = new char[len];
		in.read(g->playername, len);
		g->playername[len] = '\0';

		in.read((char*)&len, sizeof(len));
		//if (g->path)delete[] g->path;
		g->path = new char[len];
		in.read(g->path, len);
		g->path[len] = '\0';

		//wąż:
		int x,y;
		in.read((char*)&x, sizeof(int));
		in.read((char*)&y, sizeof(int));
		g->head = new SnakePart(x,y, g->width, g->scale, "Resources/head_u.png", nullptr, nullptr);
		auto w = g->head;
		for (int i = 0; i < g->snakeLength - 1; i++) {
			in.read((char*)&x, sizeof(int));
			in.read((char*)&y, sizeof(int));
			w->next = new SnakePart(x, y, g->width, g->scale, "Resources/transparent.png", nullptr, w);
			w = w->next;
		}
		in.read((char*)&x, sizeof(int));
		in.read((char*)&y, sizeof(int));
		//jabłko:
		g->apple= new Fruit(x, y, g->width, g->scale, "Resources/fruit.png", g->snakeSpeed);
		g->state = 0;
		cout << "Pomyslnie wczytano gre" << endl;
		in.close();
		return g;
	}
	cout << "Nie udalo sie wczytac gry" << endl;
	return nullptr;
}

void writeToFile(GameInfo* g) {
	fstream out;
	out.open(g->path, ios::out | ios::binary);
	if (out) {
		out.write((char*)g, sizeof(GameInfo));

		int len= strlen(g->playername);
		out.write((char*)&len, sizeof(len));
		out.write(g->playername, len);

		len = strlen(g->path);
		out.write((char*)&len, sizeof(len));
		out.write(g->path, len);

		int x, y;
		auto w = g->head;
		while (w) {
			x = w->getX();
			y = w->getY();
			out.write((char*)&x, sizeof(int));
			out.write((char*)&y, sizeof(int));
			w = w->next;
		}

		x = g->apple->getX();
		y = g->apple->getY();
		out.write((char*)&x, sizeof(int));
		out.write((char*)&y, sizeof(int));

		cout << "Pomyslnie zapisano gre" << endl;
		out.close();
	}
	else {
		cout << "Nie udalo sie zapisac gry" << g->path << endl;
	}
}

Tile** boardSetup(GameInfo* game) {
	Tile** board = new Tile * [game->size];
	for (int i = 0; i < game->size; i++) {
		board[i] = new Tile[game->size];
		for (int j = 0; j < game->size; j++) {
			board[i][j].setX(i);
			board[i][j].setY(j);
			if (i % 2 == 0) {
				if (j % 2 == 0) {
					board[i][j].loadSprite(game->scale, "Resources/light.png");
				}
				else {
					board[i][j].loadSprite(game->scale, "Resources/dark.png");
				}
			}
			else {
				if (j % 2 == 0) {
					board[i][j].loadSprite(game->scale, "Resources/dark.png");
				}
				else {
					board[i][j].loadSprite(game->scale, "Resources/light.png");
				}
			}
			board[i][j].updatePosition(game->width);
			board[i][j].isEmpty = 1;
		}
	}

	return board;
}

void Tick(GameInfo* game, Tile** board) {
	SnakePart::move(game->head);
	switch (game->direction) {
		case 0:
			game->head->setY(game->head->getY() - 1);
			break;
		case 1:
			game->head->setY(game->head->getY() + 1);
			break;
		case 2:
			game->head->setX(game->head->getX() - 1);
			break;
		case 3:
			game->head->setX(game->head->getX() + 1);
	}
	if (*game->head==*game->apple) {
		SnakePart::grow(game->head, game);
		game->snakeLength++;
		game->currentScore = game->currentScore + game->apple->getPoints();
		if (game->currentScore == game->snakeSpeed * (game->size * game->size - game->snakeStartLength + 1)) game->state = 4;
		if (game->highScore < game->currentScore) game->highScore = game->currentScore;
		updateBoardStatus(game->head, board);
		while (game->state!=4) {
			game->apple->setX(rand() % game->size);
			game->apple->setY(rand() % game->size);
			if((board[game->apple->getY()][game->apple->getX()].isEmpty)) break;
		}
		game->apple->updatePosition(game->width);
	}
	SnakePart::checkCollision(game->head, game);
	if (game->state != 3) game->head->updateSnake(game->width);
}

void updateBoardStatus(SnakePart*& head, Tile** board) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			board[i][j].isEmpty=1;
		}
	}
	auto w = head;
	while (w) {
		board[w->getY()][w->getX()].isEmpty = 0;
		w = w->next;
	}
}

ostream& operator<<(ostream& s, const GameInfo& g){
	s << " size: " << g.size << " width: " << g.width << " scale: " << g.scale << endl;
	s << " snakeLength: " << g.snakeLength << " snakeStartLength: " << g.snakeStartLength <<  " snakeSpeed: " << g.snakeSpeed << endl;
	s << " playername: " << g.playername << endl;
	s << " path: " << g.path << endl;
	s << " currentScore: " << g.currentScore << " highScore: " << g.highScore << endl;
	s << " state: " << g.state << endl;
	s << " direction: " << g.direction << endl;
	return s;
}

void deleteSnake(SnakePart*& head) {
	if (head) {
		deleteSnake(head->next);
		delete head;
		head = nullptr;
	}
}