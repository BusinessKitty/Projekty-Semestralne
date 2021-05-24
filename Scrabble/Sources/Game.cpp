#pragma warning(push, 0)
#include <SFML/Graphics.hpp>
#pragma warning(pop)

#include <set>
#include <thread>
#include <future>
#include <memory>

#include "..\Headers\Text.h"
#include "..\Headers\Drawable.h"
#include "..\Headers\Container.h"
#include "..\Headers\Bag.h"
#include "..\Headers\Tile.h"
#include "..\Headers\Blank.h"
#include "..\Headers\Slot.h"
#include "..\Headers\Board.h"
#include "..\Headers\Player.h"
#include "..\Headers\Singleton.h"
#include "..\Headers\Arbiter.h"
#include "..\Headers\Game.h"
#include "..\Headers\WindowManager.h"

Game::Game(std::string rules_, int time_) :
    window(sf::VideoMode(1575, 900), "Scrabble", sf::Style::Titlebar | sf::Style::Close),
    background(sf::Vector2f(0, 0), "Resources/textures/background.png"),
    boardBackground(sf::Vector2f(754, 754)),
    rules(rules_), time(time_), passCount(0), state(State::IDLE),
    board(std::make_shared<Board>()), bag(std::make_shared<Bag>()),
    currentPlayer(std::make_shared<Player>(2, time)), nextPlayer(std::make_shared<Player>(1, time))
{
    //załaduj ikonę i czcionkę
    icon.loadFromFile("Resources/textures/icon.png");
    font.loadFromFile("Resources/baloo_font/Baloo-Regular.ttf");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    boardBackground.setPosition(sf::Vector2f(73, 73));
    boardBackground.setFillColor(sf::Color::Black);
    //załaduj przyciski
    initializeButtons();
    //załaduj teksty
    initializeTexts();
}

Game::~Game() {}

int Game::play(){
    //ropocznij czas
    sf::Clock c;
    //zacznij pierwszą turę
    nextTurn();
    //w każdej klatce
    while (window.isOpen()) {
        //sprawdź czy gra powinna się zakończyć
        int temp = checkEndConditions();
        if (temp != 2) return temp;
        //obsłuż zdarzenia
        handleEvents(&c);
        //wyczyść okno
        window.clear();
        //rysuj tło
        window.draw(*background.getSprite());
        //rysuj tło planszy
        window.draw(boardBackground);
        //rysuj planszę
        for (std::vector<std::shared_ptr<Drawable>>::iterator it = board->begin(); it != board->end(); it++) window.draw(*(*it)->getSprite());
        //rysuj płytki na planszy
        for (std::vector<std::shared_ptr<Tile>>::iterator it = board->tilesOnBoard.begin(); it != board->tilesOnBoard.end(); it++) window.draw(*(*it)->getSprite());
        //rysuj płytki aktualnego gracza (bez podniesionej)
        for (std::vector<std::shared_ptr<Tile>>::iterator it = currentPlayer->hand.begin(); it != currentPlayer->hand.end(); it++) 
            if((*it)->getStatus() != Tile::Status::PICKED_UP) window.draw(*(*it)->getSprite());
        //rysuj przyciski
        for (std::map<std::string, std::unique_ptr<Drawable>>::iterator it = buttons.begin(); it != buttons.end(); ++it) window.draw(*it->second->getSprite());
        //rysuj teksty
        for (std::map<std::string, Text>::iterator it = texts.begin(); it != texts.end(); ++it) window.draw(*(it->second.getText()));
        //rysuj podniesioną płytkę aktualnego gracza (środek tej płytki powinien znaleźć się pod kursorem)
        for (std::vector<std::shared_ptr<Tile>>::iterator it = currentPlayer->hand.begin(); it != currentPlayer->hand.end(); it++) {
            if ((*it)->getStatus() == Tile::Status::PICKED_UP) { 
                (*it)->setCoords(sf::Vector2f(sf::Mouse::getPosition(window).x - 25, sf::Mouse::getPosition(window).y - 25));
                window.draw(*((*it)->getSprite())); 
            }
        }
        //pokaż co tam nabazgrałeś
        window.display();
    }
    return 0;
}

void Game::handleEvents(sf::Clock* c) {
    //zmniejsz czas aktualnego gracza i wyświetl go
    if (time) {
        currentPlayer->timeLeft -= c->restart();
        if (currentPlayer->id == 1) {
            texts["p1_time"].setString(sfTimeToString(&currentPlayer->timeLeft));
            texts["p1_time"].setOriginToMiddle();
            texts["p2_time"].setString(sfTimeToString(&nextPlayer->timeLeft));
            texts["p2_time"].setOriginToMiddle();
        }
        else {
            texts["p2_time"].setString(sfTimeToString(&currentPlayer->timeLeft));
            texts["p2_time"].setOriginToMiddle();
            texts["p1_time"].setString(sfTimeToString(&nextPlayer->timeLeft));
            texts["p1_time"].setOriginToMiddle();
        }
    }

    sf::Event event;
    //zarządzaj zdarzeniami
    while (window.pollEvent(event)) {
        //próba zamknięcia okna / klik na ESC
        if (state == State::IDLE && (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))) {
            //wyświetl ekran potwierdzenia wyjścia
            std::future<bool> fut = std::async(WindowManager::escapePromptScreen);
            //ustaw odpowiedni stan, jeżeli potwierdzono wyjście
            if (fut.get()) state = Game::State::SURRENDERED;
        }
        //próba poddania się
        else if (state == State::IDLE && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && buttons["surr"]->getBounds()->contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
            //wyświetl ekran potwierdzenia poddania się
            std::future<bool> fut = std::async(WindowManager::surrenderPromptScreen);
            //ustaw odpowiedni stan, jeżeli potwierdzono poddanie się
            if (fut.get()) state = Game::State::SURRENDERED;
        }
        //zagranie słowa
        else if (state == State::IDLE && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && buttons["play"]->getBounds()->contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
            //sprawdź poprawność
            currPlayerCheck = Arbiter::instance().checkWords(currentPlayer->hand, board, &texts["info"]);
            //jeżeli poprawne: dodaj punkty, zagraj, zresetuj passcount, następna tura
            if (currPlayerCheck.second != 0) {
                currentPlayer->points = currentPlayer->points + currPlayerCheck.second;
                currentPlayer->play(board);
                passCount = 0;
                nextTurn();
            }
        }
        //spasowanie
        else if (state == State::IDLE && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && buttons["pass"]->getBounds()->contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
            passCount++;
            currentPlayer->pass(board);
            std::wstring temp = L"Gracz numer ";
            temp += std::to_wstring(currentPlayer->id);
            temp += L" spasował.";
            texts["info"].setString(temp);
            texts["info"].setPosition(sf::Vector2f(450, 45));
            texts["info"].setOriginToMiddle();
            nextTurn();
        }
        //sprawdzenie liter w worku
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && buttons["check"]->getBounds()->contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
            std::thread([=] {WindowManager::tilesLeftScreen(bag); }).detach();
        }
        //rozpoczęcie wymiany
        else if (state == State::IDLE && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && buttons["swap"]->getBounds()->contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
            texts["info"].setString(L"Wymiana liter...");
            texts["info"].setPosition(sf::Vector2f(450, 45));
            texts["info"].setOriginToMiddle();
            texts["your_tiles"].setString("Zaznacz litery do wymiany:");
            texts["your_tiles"].setPosition(sf::Vector2f(904, 170));
            buttons["swap"]->setSprite("Resources/textures/confirm.png");
            currentPlayer->startSwap(board,bag);
            state = State::SWAPPING;
        }
        //zakończenie wymiany
        else if (state == State::SWAPPING && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && buttons["swap"]->getBounds()->contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
            texts["your_tiles"].setString("Twoje litery:");
            texts["your_tiles"].setPosition(sf::Vector2f(1062, 170));
            buttons["swap"]->setSprite("Resources/textures/swap.png");
            std::pair<bool,int> temp = currentPlayer->endSwap(bag);
            //jeśli wymiana nie nastąpiła
            if (!temp.first) {
                //jeśli gracz nie wymienił nic
                if (temp.second == 0) {
                    texts["info"].setString(L"Przeciągnij płytkę na planszę, aby zagrać");
                    texts["info"].setPosition(sf::Vector2f(450, 45));
                    texts["info"].setOriginToMiddle();
                }
                //jeśli gracz chciał wymienić więcej niż w worku
                else {
                    std::wstring t=L"Zamierzałeś wymienić ";
                    t += std::to_wstring(temp.second);
                    t += L" płytek. W worku jest tylko ";
                    t += std::to_wstring(bag->size());
                    t += L".";
                    texts["info"].setString(t);
                    texts["info"].setPosition(sf::Vector2f(450, 45));
                    texts["info"].setOriginToMiddle();
                }
                
            }
            //jeśli wymiana nastąpiła pomyślnie
            else {
                std::wstring t = L"Gracz numer ";
                t += std::to_wstring(currentPlayer->id);
                t += L" wymienił ";
                t += std::to_wstring(temp.second);
                if (temp.second == 1) t += L" płytkę.";
                else if (temp.second % 100 >= 0 && temp.second % 10 > 1 && temp.second % 10 < 5) t += L" płytki.";
                else t += L" płytek.";
                texts["info"].setString(t);
                texts["info"].setPosition(sf::Vector2f(450, 45));
                texts["info"].setOriginToMiddle();
                nextTurn();
                passCount = 0;
            }
            state = State::IDLE;
        }
        //kliknięcie na płytkę w trybie wymiany
        else if (state == State::SWAPPING && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            for (std::vector<std::shared_ptr<Tile>>::iterator it = currentPlayer->hand.begin(); it != currentPlayer->hand.end(); it++) {
                if ((*it)->getBounds()->contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    currentPlayer->selectForSwap(*it);
                    break;
                }
            }
        }
        //podniesienie płytki
        else if (state == State::IDLE && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            for (std::vector<std::shared_ptr<Tile>>::iterator it = currentPlayer->hand.begin(); it != currentPlayer->hand.end(); it++) {
                if ((*it)->getBounds()->contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) { 
                    currentPlayer->grab((*it)); 
                    //zaznacz że na danym miejscu na planszy nie ma już płytki
                    if ((*it)->getPositionOnBoard() != -1) board->findSlotOnPosition((*it)->getPositionOnBoard())->leaveSlot();
                    state = State::PICKED;
                    break;
                }
            }
        }
        //kliknięcie prawym żeby wrócić płytkę do ręki
        else if (state == State::IDLE && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
            for (std::vector<std::shared_ptr<Tile>>::iterator it = currentPlayer->hand.begin(); it != currentPlayer->hand.end(); it++) {
                if ((*it)->getBounds()->contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    //zaznacz że na danym miejscu na planszy nie ma już płytki
                    if ((*it)->getPositionOnBoard() != -1) board->findSlotOnPosition((*it)->getPositionOnBoard())->leaveSlot();
                    currentPlayer->backToHand(*it);
                    //sprawdź poprawność
                    currPlayerCheck = Arbiter::instance().checkWords(currentPlayer->hand, board, &texts["info"]);
                    break;
                }
            }
            
        }
        //upuszczenie płytki
        else if (state == State::PICKED && event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            for (std::vector<std::shared_ptr<Tile>>::iterator it = currentPlayer->hand.begin(); it != currentPlayer->hand.end(); it++) {
                if ((*it)->getBounds()->contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    currentPlayer->drop(board,*it);
                    //jeżeli blank oraz jeżeli został upuszczony na planszę to wybierz literę
                    if ((*it)->getLetter() == '_' && (*it)->getPositionOnBoard() != -1) {
                        std::thread([=] {WindowManager::pickLetterScreen(bag, std::static_pointer_cast<Blank>((*it))); }).join();
                    } 
                    //sprawdź poprawność
                    currPlayerCheck = Arbiter::instance().checkWords(currentPlayer->hand, board, &texts["info"]);
                    state = State::IDLE;
                    break;
                }
            }
        }
    }
}

int Game::checkEndConditions() {
    //sprawdź czy skończył się czas
    if (currentPlayer->timeLeft.asSeconds() < 0) {
        state = Game::State::TIME_OUT;
        std::future<bool> nextGameFut = std::async(WindowManager::endScreen, state, currentPlayer, nextPlayer);
        //zwróć info czy zacząć nową grę
        return nextGameFut.get();
    }
    //sprawdź czy "empty" lub "both" zostały spełnione
    if ((rules == "empty" || rules == "both") && (bag->size() == 0 && (currentPlayer->hand.empty() || nextPlayer->hand.empty()))) {
        std::future<bool> nextGameFut = std::async(WindowManager::endScreen, state, currentPlayer, nextPlayer);
        //zwróć info czy zacząć nową grę
        return nextGameFut.get();
    }
    //sprawdź czy "pass" lub "both zostały spełnione"
    if ((rules == "pass" || rules == "both") && passCount == 3) {
        std::future<bool> nextGameFut = std::async(WindowManager::endScreen, state, currentPlayer, nextPlayer);
        //zwróć info czy zacząć nową grę
        return nextGameFut.get();
    }
    //sprawdź czy gracz się poddał
    if (state == State::SURRENDERED) {
        std::future<bool> nextGameFut = std::async(WindowManager::endScreen, state, currentPlayer, nextPlayer);
        //zwróć info czy zacząć nową grę
        return nextGameFut.get();
    }
    //zwróć info że gra trwa dalej
    return 2;
}

void Game::nextTurn() {
    //zaktualizuj tekst pokazujący punkty aktualnego gracza
    if (currentPlayer->id == 1) {
        texts["p1_points"].setString(std::to_string(currentPlayer->points));
        texts["p1_points"].setOriginToMiddle();
    }
    else {
        texts["p2_points"].setString(std::to_string(currentPlayer->points));
        texts["p2_points"].setOriginToMiddle();
    }
    //zacznij turę następnego gracza
    nextPlayer->startTurn(bag);
    //zaktualizuj tekst pokazujący ilość liter w worku
    texts["tiles_left"].setString(std::to_string(bag->size()));
    //zamień currentPlayer z nextPlayer
    std::swap(currentPlayer, nextPlayer);
    //zaktualizuj tekst wyświetlający którego gracza tura aktualnie
    texts["turn"].setString(std::to_string(currentPlayer->id));
    
}

std::string Game::sfTimeToString(sf::Time* time) {
    int value = (int)time->asSeconds();
    std::string temp="";
    if (value % 60 < 10) temp += "0";
    temp += std::to_string(value % 60);
    return std::to_string(value / 60) + ":" + temp;
}

void Game::initializeButtons(){
    buttons["play"] = std::make_unique<Drawable>(sf::Vector2f(956, 420), "Resources/textures/play.png");
    buttons["swap"] = std::make_unique<Drawable>(sf::Vector2f(1092, 420), "Resources/textures/swap.png");
    buttons["check"] = std::make_unique<Drawable>(sf::Vector2f(1228, 420), "Resources/textures/check.png");
    buttons["pass"] = std::make_unique<Drawable>(sf::Vector2f(1364, 420), "Resources/textures/pass.png");
    buttons["surr"] = std::make_unique<Drawable>(sf::Vector2f(1364, 750), "Resources/textures/surr.png");
}
void Game::initializeTexts(){
    texts["turn"] = Text("1", &font, 53, sf::Vector2f(1442, 69), 0);
    texts["tiles_left"] = Text("93", &font, 27, sf::Vector2f(1291, 345), 0);
    texts["your_tiles"] = Text("Twoje litery:", &font, 48, sf::Vector2f(1062, 170), 0);
    texts["p1_points"] = Text("0", &font, 32, sf::Vector2f(1210, 633), 1);
    texts["p2_points"] = Text("0", &font, 32, sf::Vector2f(1210, 675), 1);
    texts["p1_time"] = Text("-", &font, 32, sf::Vector2f(1376, 633), 1);
    texts["p2_time"] = Text("-", &font, 32, sf::Vector2f(1376, 675), 1);
    texts["info"] = Text(L"Przeciągnij płytkę na planszę, aby zagrać", &font, 26, sf::Vector2f(450, 45), 1);
}