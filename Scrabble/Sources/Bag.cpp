#pragma warning(push, 0)
#include <SFML/Graphics.hpp>
#pragma warning(pop)
#include <cctype>
#include <algorithm>
#include <memory>

#include "..\Headers\Text.h"
#include "..\Headers\Drawable.h"
#include "..\Headers\Tile.h"
#include "..\Headers\Blank.h"
#include "..\Headers\Container.h"
#include "..\Headers\Bag.h"
#include "..\Headers\Player.h"

Bag::Bag() :Container(){
    std::srand((unsigned int)std::time(0));
    std::string name;
    for (std::map<char, int>::iterator it = quantities.begin(); it != quantities.end(); it++) {
        name = "Resources/textures/tiles/";
        if((*it).first>=97 && (*it).first <=122) name.push_back((*it).first);
        name.push_back((*it).first);
        name.append(".png");
        int points = values.find((*it).first)->second;
        for (int i = (*it).second; i > 0;i--) {
            if ((*it).first == '_') contents.push_back(std::make_shared<Blank>(sf::Vector2f(0, 0), name, (*it).first, points));
            else contents.push_back(std::make_shared<Tile>(sf::Vector2f(0, 0), name, (*it).first, points));
        }
    }
}
Bag::~Bag(){}

std::shared_ptr<Tile> Bag::grab() {
    //pomieszaj litery w worku
    std::random_shuffle(contents.begin(), contents.end());
    //wyciągnij ostatnią
    std::shared_ptr<Tile> temp = std::static_pointer_cast<Tile>(contents.back());
    //znajdź literę w quantities i zmniejsz licznik
    quantities.find(temp->getLetter())->second--;
    //usuń ostatnią z contents i zwróć graczowi
    contents.pop_back();
    return temp;
}

void Bag::putBack(std::shared_ptr<Tile> tile) {
    //znajdź literę w quantities i zmniejsz licznik
    quantities.find(tile->getLetter())->second++;
    contents.push_back(tile);

}

std::map<char, int> Bag::getQuantities() {
    return quantities;
}