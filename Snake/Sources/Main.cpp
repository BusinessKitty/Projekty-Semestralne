#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <math.h>
#include <ctime>
#include <fstream>
#include <windows.h>
using namespace std;
using namespace sf;

#include "..\Headers\GameInfo.h"
#include "..\Headers\Tile.h"
#include "..\Headers\SnakePart.h"
#include "..\Headers\Fruit.h"
#include "..\Headers\Global.h"

int main(int argc, char* argv[]) {
	srand(time(0));
	
	//czcionka:
	sf::Font font;
	font.loadFromFile("Resources/baloo_font/Baloo-Regular.ttf");
	//ikona pause:
	Texture pauseT;
	pauseT.loadFromFile("Resources/pause.png");
	Sprite pause(pauseT);
	pause.setPosition(144, 144);
	pause.setColor(Color(255, 255, 255, 64));
	//ikona play:
	float fade = 64;
	Texture playT;
	playT.loadFromFile("Resources/play.png");
	Sprite play(playT);
	play.setPosition(144, 144);
	play.setColor(Color(255, 255, 255, fade));
	//Background do start / lost / won:
	RectangleShape bg(Vector2f(576, 576));
	bg.setPosition(72, 72);
	bg.setFillColor(Color(255,255,255,127));
	//tekst start:
	Text start(L"Kliknij spację, aby zacząć grę.",font,34);
	start.setOrigin(start.getLocalBounds().left + start.getLocalBounds().width/2, start.getLocalBounds().top + start.getLocalBounds().height);
	start.setFillColor(sf::Color::Black);
	start.setPosition(360,342);
	//tekst lost:
	Text l1(L"Przegrałeś!", font, 34);
	l1.setOrigin(l1.getLocalBounds().left + l1.getLocalBounds().width / 2, l1.getLocalBounds().top + l1.getLocalBounds().height);
	l1.setFillColor(sf::Color::Black);
	l1.setPosition(360, 306);
	Text l2(L"Kliknij esc, aby wyjść z gry.", font, 34);
	l2.setOrigin(l2.getLocalBounds().left + l2.getLocalBounds().width / 2, l2.getLocalBounds().top + l2.getLocalBounds().height);
	l2.setFillColor(sf::Color::Black);
	l2.setPosition(360, 378);
	Text l3(L"Kliknij space, aby zagrać ponownie.", font, 34);
	l3.setOrigin(l3.getLocalBounds().left + l3.getLocalBounds().width / 2, l2.getLocalBounds().top + l2.getLocalBounds().height);
	l3.setFillColor(sf::Color::Black);
	l3.setPosition(360, 450);
	//tekst won:
	Text w1(L"Wygrałeś!", font, 34);
	w1.setOrigin(w1.getLocalBounds().left + w1.getLocalBounds().width / 2, w1.getLocalBounds().top + w1.getLocalBounds().height);
	w1.setFillColor(sf::Color::Black);
	w1.setPosition(360, 342);
	//Background do pola gry i ekran instrukcji:
	RectangleShape rectangle(Vector2f(580, 580));
	rectangle.setPosition(70, 70);
	rectangle.setFillColor(Color::Black);
	Texture instrukcjeT;
	instrukcjeT.loadFromFile("Resources/instrukcje.png");
	Sprite instrukcje(instrukcjeT);
	instrukcje.setPosition(Vector2f(720, 72));
	//Gracz:
	Text playername("", font, 36);
	playername.setFillColor(sf::Color::Black);
	playername.setPosition(900, 395);
	//Gra:
	Text gra("", font, 36);
	gra.setFillColor(sf::Color::Black);
	gra.setPosition(900, 453);
	//Aktualny wynik:
	Text score("", font, 36);
	score.setFillColor(sf::Color::Black);
	score.setPosition(1080, 511);
	//Najlepszy wynik:
	Text highscore("", font, 36);
	highscore.setFillColor(sf::Color::Black);
	highscore.setPosition(1080, 569);

	bool fromFile = 0;
	//gra:
	GameInfo* game;
	//jeśli użytkownik otworzył program jako: .\snake.exe, utwórz grę z domyślnymi parametrami
	if (argc == 1) { 
		game = new GameInfo(); 
		game->head = new SnakePart((game->size) - 1, (game->size) - 1, game->width, game->scale, "Resources/head_u.png", nullptr, nullptr);
		auto w = game->head;
		for (int i = 0; i < game->snakeLength - 1; i++) {
			w->next = new SnakePart(w->getX(), w->getY(), game->width, game->scale, "Resources/transparent.png", nullptr, w);
			w = w->next;
		}
	}
	//jeśli użytkownik otworzył program jako: .\snake.exe -game <nazwa_gry> (tylko jeden parametr: -game), wczytaj grę z pliku
	else if (argc == 3 && !strcmp(argv[1], "-game") && argv[2]) {
		//jesli <nazwa_gry> poprawne
		if (argv[2][0] != '-') { // TODO SPRAWDZANIE POPRAWNOSCI NAZWY PLIKU
			char* path;
			//zarezerwuj pamięć dla "Resources/Saves/<nazwa_gry>.bin" albo "Resources/Saves/<nazwa_gry>"
			path = new char[21 + strlen(argv[2])];
			strcpy_s(path, 17, "Resources/Saves/");
			strcat(path, argv[2]);
			//jeśli nazwa podana przez użytkownika nie zawierała rozszerzenia .bin, dodaj je:
			if (!(path[15 + strlen(argv[2])] == 'n' && path[14 + strlen(argv[2])] == 'i' && path[13 + strlen(argv[2])] == 'b' && path[12 + strlen(argv[2])] == '.')) strcat(path, ".bin");
			game = gameSetup(path);
			fromFile = 1;
			delete[] path;
		}
		//jeśli <nazwa_gry> niepoprawne
		else {
			cout << "podano zly parametr przelacznika -game!" << endl;
			game = nullptr;
		}
	}
	//jeśli użytkownik wpisał wiele parametrów, utwórz nową grę z podanymi parametrami
	else game = gameSetup(argc, argv);

	if (!game) return 0;


	//nazwa gry:
	char* nazwa_gry;
	int suma = 0;
	if (strcmp(game->path, "-")) {
		for (int i = 16; i < strlen(game->path); i++) {
			if (game->path[i] == '.') break;
			suma++;
		}
		nazwa_gry = new char[suma + 1];
		for (int i = 16; i < 16 + suma; i++) {
			nazwa_gry[i - 16] = game->path[i];
		}
		nazwa_gry[suma] = '\0';
	}
	else {
		nazwa_gry = new char[2];
		strcpy_s(nazwa_gry, 2, "-");
	}
	gra.setString(nazwa_gry);
	delete[] nazwa_gry;
	//plansza:
	Tile** board = boardSetup(game);
	int jtime;
	jtime = game->snakeStartLength;
	if(fromFile && game->snakeLength!=game->snakeStartLength)jtime = 0;
	//zegar, timer, opóźnienie:
	Clock clock;
	float timer = 0, delay = 1/(pow(2,game->snakeSpeed));

	//okno:
	RenderWindow window(VideoMode(1280, 720), "Snake!");

	while (window.isOpen()) {
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) {
				window.close();
				game->updateScoreboard();
				if (strcmp(game->path, "-") && game->state != 3 && game->state != 4) writeToFile(game);
				for (int i = 0; i < game->size; ++i) delete[] board[i];
				delete[] board;
				delete game;
				return 0;
			}
			if (e.type == Event::KeyPressed) {
				//jeśli ESCAPE
				if (e.key.code == Keyboard::Escape) {
					window.close();
					game->updateScoreboard();
					if (strcmp(game->path, "-") && game->state !=3 && game->state != 4) writeToFile(game);
					for (int i = 0; i < game->size; ++i) delete[] board[i];
					delete[] board;
					delete game;
					return 0;
				}
				//jeśli SPACE
				if (e.key.code == Keyboard::Space) {
					//jeśli aktualny stan to 'start' albo 'pauza', ustaw na 'play'
					if (game->state == 0 || game->state ==2) game->state = 1;
					//jeśli aktualny stan to 'play', ustaw na 'pauza'
					else if (game->state == 1) {
						game->state = 0; 
						fade = 64;
					}
				}
				//jeśli aktualny stan to 'play' - obsługuj sterowanie
				if (game->state == 1) {
					if (e.key.code == Keyboard::Down || e.key.code == Keyboard::S) {
						if (game->direction != 0 && game->direction != 1) {
							game->direction = 1;
						}
					}
					if (e.key.code == Keyboard::Left || e.key.code == Keyboard::A) {
						if (game->direction != 3 && game->direction != 2) {
							game->direction = 2;
						}
					}
					if (e.key.code == Keyboard::Right || e.key.code == Keyboard::D) {
						if (game->direction != 3 && game->direction != 2) {
							game->direction = 3;
						}
					}
					if (e.key.code == Keyboard::Up || e.key.code == Keyboard::W) {
						if (game->direction != 0 && game->direction != 1) {
							game->direction = 0;
						}
					}
				}
				//jeśli aktualny stan to 'lost' i użytkownik kliknie spację - play again
				if (game->state == 3 && e.key.code == Keyboard::Space) {
					GameInfo* temp = new GameInfo(game);
					delete game;
					game = nullptr;
					game = new GameInfo(temp);
					delete temp;
					temp = nullptr;
					game->head = new SnakePart((game->size) - 1, (game->size) - 1, game->width, game->scale, "Resources/head_u.png", nullptr, nullptr);
					for (int i = 0; i < game->snakeLength - 1; i++) SnakePart::grow(game->head, game);
					jtime = game->snakeStartLength;
				}
			}
		}
		//jeśli aktualny stan to 'play', wykonuj Tick po odpowiednim opóźnieniu
		if (game->state == 1) {
			if (timer > delay) {
				timer = 0;
				Tick(game,board);
				if (jtime > -1)jtime--;
			}
		}

		//RYSUJ:

		//okno wypełnione niebieskim:
		window.clear(Color::Cyan);
		//tło pod planszę:
		window.draw(rectangle);
		//ekran instrukcji:
		window.draw(instrukcje);
		//jaskinia:
		if (jtime == game->snakeStartLength) { 
			board[game->size - 1][game->size - 2].loadSprite(game->scale, "Resources/head_s.png");
			board[game->size - 1][game->size - 1].loadSprite(game->scale, "Resources/cave.png"); 
		}
		else if (jtime == game->snakeStartLength - 1) {
			board[game->size - 1][game->size - 2].loadSprite(game->scale, "Resources/dark.png");
		}
		else if (jtime == 0){ 
			board[game->size - 1][game->size - 1].loadSprite(game->scale, "Resources/light.png");
		}
		//plansza:
		if (board) {
			for (int i = 0; i < game->size; i++) {
				for (int j = 0; j < game->size; j++) {
					window.draw(*(board[i][j].getSprite()));
				}
			}
		}
		//wąż:
		if (game->state != 3) SnakePart::draw(game->head,&window,game->scale);
		else {
			auto w = game->head;
			while (w) {
				window.draw(*w->getSprite());
				w = w->next;
			}
		}
		if (jtime > 0) window.draw(*(board[game->size - 1][game->size - 1].getSprite()));
		if(jtime==game->snakeStartLength) window.draw(*(board[game->size - 1][game->size - 2].getSprite()));
		//owoc:
		if(game->state!=4) window.draw(*(game->apple->getSprite()));
		//gracz:
		playername.setString(game->playername);
		window.draw(playername);
		//gra::
		window.draw(gra);
		//aktualny wynik:
		score.setString(to_string(game->currentScore));
		window.draw(score);
		//najlepszy wynik:
		highscore.setString(to_string(game->highScore));
		window.draw(highscore);
		//pause:
		if (game->state == 0) window.draw(pause);
		//play:
		if (game->state == 1 && fade>0) {
			window.draw(play);
			play.setColor(Color(255, 255, 255, fade));
			fade=fade-1;
		}
		//start:
		if (game->state == 2) {
			window.draw(bg);
			window.draw(start);
		}
		//lost
		if (game->state == 3) {
			window.draw(bg);
			window.draw(l1);
			window.draw(l2);
			window.draw(l3);
		}
		//won
		if (game->state == 4) {
			window.draw(bg);
			window.draw(w1);
			l2.setPosition(360, 414);
			window.draw(l2);
		}
		window.display();
	}
	return 0;
}