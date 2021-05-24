#pragma warning(push, 0)
#include <SFML/Graphics.hpp>
#pragma warning(pop)

#include "..\Headers\Text.h"
#include "..\Headers\Drawable.h"
#include "..\Headers\Slot.h"
#include "..\Headers\Container.h"
#include "..\Headers\Board.h"
#include "..\Headers\Player.h"

Board::Board() :Container(){
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            //srodek - mnoznik slowny x2:
            if (i == 7 && j == 7)contents.push_back(std::make_shared<Slot>(sf::Vector2f((float)75 + j * 50, (float)75 + i * 50), "Resources/textures/slots/Center.png", std::make_pair<int,Slot::MultiplierType>(2, Slot::MultiplierType::WORD)));
            //mnozniki slowne x3:
            else if ((j == 0 || j == 7 || j == 14) && i % 7 == 0)contents.push_back(std::make_shared<Slot>(sf::Vector2f((float)75 + j * 50, (float)75 + i * 50), "Resources/textures/slots/WordX3.png", std::make_pair<int, Slot::MultiplierType>(3, Slot::MultiplierType::WORD)));
            //mnozniki slowne x2:
            else if (((j > 0 && j < 5) || (j > 9 && j < 14)) && (j == i || j == 14 - i))contents.push_back(std::make_shared<Slot>(sf::Vector2f((float)75 + j * 50, (float)75 + i * 50), "Resources/textures/slots/WordX2.png", std::make_pair<int, Slot::MultiplierType>(2, Slot::MultiplierType::WORD)));
            //mnozniki literowe x3:
            else if (i % 4 == 1 && j % 4 == 1)contents.push_back(std::make_shared<Slot>(sf::Vector2f((float)75 + j * 50, (float)75 + i * 50), "Resources/textures/slots/LetterX3.png", std::make_pair<int, Slot::MultiplierType>(3, Slot::MultiplierType::LETTER)));
            //mnozniki literowe x2:
            else if (((i == 0 || i == 3 || i == 7 || i == 11 || i == 14) && (j == 0 || j == 3 || j == 7 || j == 11 || j == 14)) || ((i == 2 || i == 6 || i == 8 || i == 12) && (j == 2 || j == 6 || j == 8 || j == 12)))
                contents.push_back(std::make_shared<Slot>(sf::Vector2f((float)75 + j * 50, (float)75 + i * 50), "Resources/textures/slots/LetterX2.png", std::make_pair<int, Slot::MultiplierType>(2, Slot::MultiplierType::LETTER)));
            //mnozniki literowe x1:
            else contents.push_back(std::make_shared<Slot>(sf::Vector2f((float)75 + j * 50, (float)75 + i * 50), "Resources/textures/slots/BasicTile.png", std::make_pair<int, Slot::MultiplierType>(1, Slot::MultiplierType::LETTER)));
        }
    }
}
Board::~Board() {};

std::shared_ptr<Slot> Board::findSlotOnPosition(const int& n) {
    for (std::vector<std::shared_ptr<Drawable>>::iterator it = contents.begin(); it != contents.end(); it++) {
        if (std::static_pointer_cast<Slot>(*it)->getPositionOnBoard() == n) return std::static_pointer_cast<Slot>(*it);
    }
    return std::shared_ptr<Slot>();
}