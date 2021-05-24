#pragma warning(push, 0)
#include <SFML/Graphics.hpp>
#pragma warning(pop)
#include "..\Headers\Text.h"
#include "..\Headers\Drawable.h"
#include "..\Headers\Tile.h"
#include "..\Headers\Blank.h"
#include "..\Headers\Slot.h"
#include "..\Headers\Container.h"
#include "..\Headers\Bag.h"
#include "..\Headers\Board.h"
#include "..\Headers\Player.h"
#include "..\Headers\Singleton.h"
#include "..\Headers\Arbiter.h"
#include "..\Headers\Game.h"
#include "..\Headers\WindowManager.h"

Drawable WindowManager::background(sf::Vector2f(0, 0), "Resources/textures/background.png");;
sf::Font WindowManager::font;
sf::Image WindowManager::icon;

WindowManager::WindowManager(token) {
	font.loadFromFile("Resources/baloo_font/Baloo-Regular.ttf");
	icon.loadFromFile("Resources/textures/icon.png");
}
WindowManager::~WindowManager() {}

void WindowManager::loadingScreen() {
	
	sf::RenderWindow w(sf::VideoMode(450, 250), L"Ładowanie Gry..", sf::Style::Titlebar);
	w.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	w.setActive(true);
	w.requestFocus();

	sf::RectangleShape loadingFrame(sf::Vector2f(300, 50));
	loadingFrame.setFillColor(sf::Color(255, 255, 255, 0));
	loadingFrame.setOutlineThickness(5);
	loadingFrame.setOutlineColor(sf::Color::Black);
	loadingFrame.setPosition(75, 150);

	srand(time(NULL));
	sf::RectangleShape loading(sf::Vector2f(0, 50));
	loading.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
	loading.setPosition(75, 150);
	
	Text first(L"Ładowanie słowników", &font, 36, sf::Vector2f(225, 30),1);
	Text second("", &font, 36,sf::Vector2f(225,85),1);
	
	float counter = 0;
	float temp;

	while (w.isOpen()) {
		temp = Arbiter::instance().setsLoaded;

		if (temp == 0) second.setString(L"A-F");
		else if (temp == 1) second.setString(L"G-M");
		else if (temp == 2) second.setString(L"N");
		else if (temp == 3) second.setString(L"O-P");
		else if (temp == 4) second.setString(L"R-U");
		else second.setString(L"W-Ż");
		second.setOriginToMiddle();

		counter = counter + 0.002;
		if (counter >= 298)counter = 298;
		else if (counter < temp * 50)counter = temp * 50;
		loading.setSize(sf::Vector2f(counter, 50));

		sf::Event e;
		while (w.pollEvent(e)) {}

		w.clear();
		w.draw(*background.getSprite());
		w.draw(loadingFrame);
		w.draw(loading);
		w.draw(*first.getText());
		w.draw(*second.getText());

		w.display();

		if(temp==6)w.close();
	}
}

std::pair<std::string,int> WindowManager::chooseConditionScreen(){
	
	sf::RenderWindow w(sf::VideoMode(900, 500), L"Wybierz zasady gry", sf::Style::Titlebar);
	w.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	w.setActive(true);
	w.requestFocus();

	std::vector<Text> texts{
		Text(L"Gra kończy się, gdy wystąpią", &font, 36,sf::Vector2f(20,20),0),
		Text(L"trzy spasowania z rzędu.", &font, 36, sf::Vector2f(20, 60), 0),
		Text(L"Gra kończy się, gdy skończą się płytki", &font, 36, sf::Vector2f(20, 140), 0),
		Text(L"w worku, oraz w ręce jednego z graczy.", &font, 36, sf::Vector2f(20, 180), 0),
		Text(L"Czas dostępny dla każdego z graczy:", &font, 36, sf::Vector2f(20, 260), 0),
		Text(L"trzy spasowania z rzędu.", &font, 36, sf::Vector2f(20, 60), 0),
		Text(L"15 minut", &font, 36, sf::Vector2f(690, 260), 0)
	};

	Drawable one(sf::Vector2f(720, 38), "Resources/textures/checked.png");
	Drawable two(sf::Vector2f(720,158), "Resources/textures/checked.png");
	Drawable leftArrow(sf::Vector2f(640, 266), "Resources/textures/arrow.png");
	Drawable rightArrow(sf::Vector2f(880, 306), "Resources/textures/arrow.png");
	rightArrow.getSprite()->setRotation(180);
	rightArrow.setCoords(sf::Vector2f(880, 306));
	Drawable confirm(sf::Vector2f(720, 360), "Resources/textures/confirm.png");
	confirm.getSprite()->setScale(0.75, 0.75);

	bool checkEmpty = 1, checkPass = 1;
	std::vector<int> times { 0,10,15,20,25,30 };
	std::vector<int>::iterator time = times.begin();

	while (w.isOpen())
	{
		sf::Event e;
		while (w.pollEvent(e)) {
			if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
				if (confirm.getBounds()->contains(w.mapPixelToCoords(sf::Mouse::getPosition(w)))) {
					w.close();
					std::string temp;
					if (checkEmpty && checkPass)temp = "both";
					else if (checkPass) temp = "pass";
					else if (checkEmpty) temp = "empty";
					return std::make_pair(temp,*time);
				} else if (leftArrow.getBounds()->contains(w.mapPixelToCoords(sf::Mouse::getPosition(w)))) {
					if (time == times.begin()) time = times.end()-1;
					else time--;
				} else if (rightArrow.getBounds()->contains(w.mapPixelToCoords(sf::Mouse::getPosition(w)))) {
					if (time == times.end()-1) time = times.begin();
					else time++;
				} else if (two.getBounds()->contains(w.mapPixelToCoords(sf::Mouse::getPosition(w)))) {
					if (checkEmpty && checkPass) {
						two.setSprite("Resources/textures/empty.png");
						checkEmpty = 0;
					}
					else if(!checkEmpty){ 
						two.setSprite("Resources/textures/checked.png");
						checkEmpty = 1;
					}
				} else if (one.getBounds()->contains(w.mapPixelToCoords(sf::Mouse::getPosition(w)))) {
					if (checkEmpty && checkPass) {
						one.setSprite("Resources/textures/empty.png");
						checkPass = 0;
					}
					else if(!checkPass){
						one.setSprite("Resources/textures/checked.png");
						checkPass = 1;
					}
				}
			}
		}

		if (time == times.begin()) { 
			texts.back().setString(L"∞");
			texts.back().setPosition(sf::Vector2f(760, 264));
		}
		else {
			std::string temp = std::to_string(*time);
			temp += " minut";
			texts.back().setString(temp);
			texts.back().setPosition(sf::Vector2f(760, 274));
		}
		texts.back().setOriginToMiddle();

		w.clear();

		w.draw(*background.getSprite());
		for (std::vector<Text>::iterator it = texts.begin(); it != texts.end(); it++)w.draw(*it->getText());
		w.draw(*confirm.getSprite());
		w.draw(*leftArrow.getSprite());
		w.draw(*rightArrow.getSprite());
		w.draw(*one.getSprite());
		w.draw(*two.getSprite());
		w.display();
	}

}

void WindowManager::tilesLeftScreen(std::shared_ptr<Bag> bag){
	sf::RenderWindow w(sf::VideoMode(550, 300), L"Pozostałe litery", sf::Style::Titlebar);
	w.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	w.setActive(true);
	w.requestFocus();

	Drawable confirm(sf::Vector2f(435, 220), "Resources/textures/confirm.png");
	confirm.getSprite()->setScale(0.8, 0.8);

	std::vector<Text> texts;
	std::map<char, int> quantities = bag->getQuantities();
	int i = 0;
	for (std::map<char, int>::iterator it = quantities.begin(); it != quantities.end(); it++) {
		std::string temp = "";
		temp.push_back((*it).first);
		temp += ": ";
		temp += std::to_string((*it).second);
		std::wstring result = Arbiter::instance().toPolish(temp);
		if (i == 29)i++;
		texts.push_back(Text(result, &font, 36,sf::Vector2f(20 + (i % 5) * 105, 5 + (i / 5) * 40),0));
		i++;
	}

	while (w.isOpen())
	{
		sf::Event e;
		while (w.pollEvent(e)) {
			if (!w.hasFocus()) w.close();
			if ((e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape) || (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left && confirm.getBounds()->contains(w.mapPixelToCoords(sf::Mouse::getPosition(w))))) {
				w.close();
			}
		}

		w.clear();

		w.draw(*background.getSprite());
		w.draw(*confirm.getSprite());
		for (std::vector<Text>::iterator it = texts.begin(); it != texts.end(); it++) {
			w.draw(*it->getText());
		}
		w.display();
	}
	return;
}

bool WindowManager::escapePromptScreen(){
	sf::RenderWindow w(sf::VideoMode(800, 240), L"Czy na pewno chcesz wyjść?", sf::Style::Titlebar);
	w.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	w.setActive(true);
	w.requestFocus();

	Text first(L"Wyjście z gry jest równoznaczne z poddaniem się.", &font, 33, sf::Vector2f(400, 20), 1);
	Text second(L"Czy na pewno chcesz wyjść?", &font, 33, sf::Vector2f(400,70), 1);

	Drawable confirm(sf::Vector2f(510, 125), "Resources/textures/confirm.png");
	Drawable deny(sf::Vector2f(210, 125), "Resources/textures/deny.png");

	while (w.isOpen())
	{
		sf::Event e;
		while (w.pollEvent(e)) {
			if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left && confirm.getBounds()->contains(w.mapPixelToCoords(sf::Mouse::getPosition(w)))) {
				w.close();
				return 1;
			}
			else if ((e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape) || (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left && deny.getBounds()->contains(w.mapPixelToCoords(sf::Mouse::getPosition(w))))) {
				w.close();
				return 0;
			}
		}

		w.clear();

		w.draw(*background.getSprite());
		w.draw(*first.getText());
		w.draw(*second.getText());
		w.draw(*deny.getSprite());
		w.draw(*confirm.getSprite());

		w.display();
	}
	return 0;
}

bool WindowManager::surrenderPromptScreen(){
	sf::RenderWindow w(sf::VideoMode(600, 180), L"Czy na pewno chcesz się poddać?", sf::Style::Titlebar);
	w.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	w.setActive(true);
	w.requestFocus();

	Text first(L"Czy na pewno chcesz się poddać?", &font, 33, sf::Vector2f(300, 20), 1);

	Drawable confirm(sf::Vector2f(360, 70), "Resources/textures/confirm.png");
	Drawable deny(sf::Vector2f(160, 70), "Resources/textures/deny.png");

	while (w.isOpen())
	{
		sf::Event e;
		while (w.pollEvent(e)) {
			if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left && confirm.getBounds()->contains(w.mapPixelToCoords(sf::Mouse::getPosition(w)))) {
				w.close();
				return 1;
			}
			else if ((e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape) || (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left && deny.getBounds()->contains(w.mapPixelToCoords(sf::Mouse::getPosition(w))))) {
				w.close();
				return 0;
			}
		}

		w.clear();

		w.draw(*background.getSprite());
		w.draw(*first.getText());
		w.draw(*deny.getSprite());
		w.draw(*confirm.getSprite());

		w.display();
	}
	return 0;
}
bool WindowManager::endScreen(const Game::State& state, std::shared_ptr<Player> currentPlayer, std::shared_ptr<Player> nextPlayer) {
	sf::RenderWindow w(sf::VideoMode(800, 300), "Koniec Gry", sf::Style::Titlebar);
	w.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	w.setActive(true);
	w.requestFocus();

	Text first("", &font, 33, sf::Vector2f(400, 20), 1);
	Text second("", &font, 33, sf::Vector2f(400, 70), 1);
	Text third(L"Rozpocząć nową grę?", &font, 33, sf::Vector2f(400, 120), 1);

	Drawable confirm(sf::Vector2f(485, 185), "Resources/textures/confirm.png");
	Drawable deny(sf::Vector2f(235, 185), "Resources/textures/deny.png");

	std::wstring temp;
	if (state == Game::State::SURRENDERED) {
		temp = L"Gracz numer ";
		temp += std::to_wstring(currentPlayer->id);
		temp += L" poddał grę.";
		first.setString(temp);
		first.setOriginToMiddle();

		temp = L"Gracz numer ";
		temp += std::to_wstring(nextPlayer->id);
		temp += L" wygrywa!";
		second.setString(temp);
		second.setOriginToMiddle();
	}
	else if (state == Game::State::TIME_OUT) {
		temp = L"Czas gracza numer ";
		temp += std::to_wstring(currentPlayer->id);
		temp += L" skończył się.";
		first.setString(temp);
		first.setOriginToMiddle();

		temp = L"Gracz numer ";
		temp += std::to_wstring(nextPlayer->id);
		temp += L" wygrywa!";
		second.setString(temp);
		second.setOriginToMiddle();
	}
	else {
		if (currentPlayer->points >= nextPlayer->points) {
			temp = L"Gracz numer ";
			temp += std::to_wstring(currentPlayer->id);
			temp += L" wygrywa zdobywając ";
			temp += std::to_wstring(currentPlayer->points);
			if (currentPlayer->points % 100 >= 0 && currentPlayer->points % 10 > 1 && currentPlayer->points % 10 < 5) temp += L" punkty!";
			else temp += L" punktów!";
			first.setString(temp);
			first.setOriginToMiddle();

			temp = L"Gracz numer ";
			temp += std::to_wstring(nextPlayer->id);
			temp += L" przegrywa zdobywając ";
			temp += std::to_wstring(nextPlayer->points);
			if (nextPlayer->points % 100 >= 0 && nextPlayer->points % 10 > 1 && nextPlayer->points % 10 < 5) temp += L" punkty.";
			else temp += L" punktów.";
			second.setString(temp);
			second.setOriginToMiddle();

		}
		else if (currentPlayer->points < nextPlayer->points) {
			temp = L"Gracz numer ";
			temp += std::to_wstring(nextPlayer->id);
			temp += L" wygrywa zdobywając ";
			temp += std::to_wstring(nextPlayer->points);
			if (nextPlayer->points % 100 >= 0 && nextPlayer->points % 10 > 1 && nextPlayer->points % 10 < 5) temp += L" punkty";
			else temp += L" punktów!";
			first.setString(temp);
			first.setOriginToMiddle();

			temp = L"Gracz numer ";
			temp += std::to_wstring(currentPlayer->id);
			temp += L" przegrywa zdobywając ";
			temp += std::to_wstring(currentPlayer->points);
			if (currentPlayer->points % 100 >= 0 && currentPlayer->points % 10 > 1 && currentPlayer->points % 10 < 5) temp += L" punkty.";
			else temp += L" punktów.";
			second.setString(temp);
			second.setOriginToMiddle();
		}
	}

	while (w.isOpen())
	{
		sf::Event e;
		while (w.pollEvent(e)) {
			if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left && confirm.getBounds()->contains(w.mapPixelToCoords(sf::Mouse::getPosition(w)))) {
				return 1;
				w.close();
			}
			else if ((e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape) || (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left && deny.getBounds()->contains(w.mapPixelToCoords(sf::Mouse::getPosition(w))))) {
				return 0;
				w.close();
			}
		}

		w.clear();

		w.draw(*background.getSprite());
		w.draw(*first.getText());
		w.draw(*second.getText());
		w.draw(*third.getText());
		w.draw(*deny.getSprite());
		w.draw(*confirm.getSprite());

		w.display();
	}
	return 0;
}
void WindowManager::pickLetterScreen(std::shared_ptr<Bag> bag, std::shared_ptr<Blank> blank){
	sf::RenderWindow w(sf::VideoMode(400, 345), L"Wybierz literę na miejsce \"_\"", sf::Style::Titlebar);
	w.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	w.setActive(true);
	w.requestFocus();

	std::map<char,Text> texts;
	std::map<char, int> quantities = bag->getQuantities();
	int i = 0;
	for (std::map<char, int>::iterator it = quantities.begin(); it != quantities.end(); it++) {
		if ((*it).first != '_') {
			std::string temp = "";
			temp.push_back((*it).first);
			std::wstring result = Arbiter::instance().toPolish(temp);
			texts[(*it).first]=Text(result, &font, 36,sf::Vector2f(20 + (i % 4) * 105, 5 + (i / 4) * 40),0);
			i++;
		}
	}

	while (w.isOpen())
	{
		sf::Event e;
		while (w.pollEvent(e)) {
			if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
				for (std::map<char, Text>::iterator it = texts.begin(); it != texts.end(); it++) {
					if (it->second.getText()->getGlobalBounds().contains(w.mapPixelToCoords(sf::Mouse::getPosition(w)))) {
						std::string name = "Resources/textures/tiles/";
						if (it->first >= 97 && it->first <= 122) name.push_back(it->first);
						name.push_back(it->first);
						name.append(".png");
						blank->setPicked(it->first);
						blank->setSprite(name);
						w.close();
					}
				}
			}
		}

		w.clear();

		w.draw(*background.getSprite());
		for (std::map<char, Text>::iterator it = texts.begin(); it != texts.end(); it++) {
			w.draw(*it->second.getText());
		}
		w.display();
	}
	return;
}