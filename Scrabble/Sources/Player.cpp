#pragma warning(push, 0)
#include <SFML/Graphics.hpp>
#pragma warning(pop)
#include <thread>

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

Player::Player(int id_,int timeLeft_) :points(0), id(id_) , grabbedFrom(0), timeLeft(sf::seconds((float)timeLeft_ * 60)){}
Player::~Player(){}

void Player::startTurn(std::shared_ptr<Bag> bag) {
	//ile p³ytek wzi¹æ z worka
	int temp = 7 - hand.size();
	//je¿eli w worku mniej ni¿ temp p³ytek, weŸ wszystkie jakie zosta³y
	if (bag->size() < temp) temp = bag->size();
	//weŸ odpowiedni¹ iloœæ p³ytek z worka i ustaw ich status na IN_HAND
	for (int i = 0; i < temp; i++) {
		hand.push_back(bag->grab());
		hand.back()->setStatus(Tile::Status::IN_HAND);
	}
	//ustaw odpowiednio koordynaty p³ytek
	for (unsigned int i = 0; i < hand.size(); i++) {
		hand[i]->setCoords(sf::Vector2f(900 + 91.5 * i, 260));
		hand[i]->setPositionOnBoard(1);
	}
}

void Player::grab(std::shared_ptr<Tile> tile) {
	//ustaw status podniesionej p³ytki na PICKED_UP
	tile->setStatus(Tile::Status::PICKED_UP);
	//ustaw grabbedFrom na pozycjê z której podnios³eœ p³ytkê
	for (unsigned int i = 0; i < hand.size(); i++) {
		if (hand[i]->getCoords() == tile->getCoords()) {
			grabbedFrom = i;
			break;
		}
	}
}

void Player::drop(std::shared_ptr<Board> board, std::shared_ptr<Tile> tile) {
	//je¿eli p³ytka zosta³a upuszczona na planszê
	if (tile->getCoords().x > 50 && tile->getCoords().y > 50 && tile->getCoords().x < 825 && tile->getCoords().y < 825) {
		//sprawdŸ nad jaki slot
		std::shared_ptr<Slot> temp;
		for (std::vector<std::shared_ptr<Drawable>>::iterator it = board->begin(); it != board->end(); it++) {
			if ((*it)->getBounds()->contains(sf::Vector2f(tile->getCoords().x + 25, tile->getCoords().y + 25)) && !std::static_pointer_cast<Slot>((*it))->isTaken()) {
				temp = std::static_pointer_cast<Slot>((*it));
				break;
			}
		}
		//jeœli slot nie by³ zajêty
		if (temp) {
			//ustaw odpowiednie koordynaty, status i pozycjê na planszy
			tile->setCoords(temp->getCoords());
			tile->setStatus(Tile::Status::TEMP_ON_BOARD);
			tile->setPositionOnBoard();
			//zabierz miejsce na planszy
			temp->takeSlot();
			//tyle
			return;
		}
	}

	//je¿eli p³ytka zosta³a upuszczona poza planszê / slot by³ zajêty - wróæ j¹ na rêkê
	tile->setCoords(sf::Vector2f(900 + 91.5 * grabbedFrom, 260));
	tile->setStatus(Tile::Status::IN_HAND);
	tile->setPositionOnBoard(1);

	//jeœli jest blankiem to z powrotem do '_' i Sprite'a
	if (tile->getLetter() == '_') {
		std::static_pointer_cast<Blank>(tile)->setPicked('_');
		tile->setSprite("Resources/textures/tiles/_.png");
	}
}

void Player::backToHand(std::shared_ptr<Tile> tile) {
	for (unsigned int i = 0; i < hand.size(); i++) {
		if (hand[i]->getCoords()==tile->getCoords()) {
			tile->setCoords(sf::Vector2f(900 + 91.5 * i, 260));
			tile->setStatus(Tile::Status::IN_HAND);
			tile ->setPositionOnBoard(1);
			//jeœli jest blankiem to z powrotem do '_' i Sprite'a
			if (tile->getLetter() == '_') {
				std::static_pointer_cast<Blank>(tile)->setPicked('_');
				tile->setSprite("Resources/textures/tiles/_.png");
			}
			break;
		}
	}
}

void Player::startSwap(std::shared_ptr<Board> board, std::shared_ptr<Bag> bag) {
	for (unsigned int i = 0; i < hand.size(); i++) {
		if (hand[i]->getStatus() == Tile::Status::TEMP_ON_BOARD) {
			//jeœli jest blankiem to z powrotem do '_' i Sprite'a
			if (hand[i]->getLetter() == '_') {
				std::static_pointer_cast<Blank>(hand[i])->setPicked('_');
				hand[i]->setSprite("Resources/textures/tiles/_.png");
			}
			board->findSlotOnPosition(hand[i]->getPositionOnBoard())->leaveSlot();
			hand[i]->setCoords(sf::Vector2f(900 + 91.5 * i, 260));
			hand[i]->setStatus(Tile::Status::IN_HAND);
			hand[i]->setPositionOnBoard(1);
		}
		hand[i]->getSprite()->setColor(sf::Color(255, 255, 255, 127));
	}
}

void Player::selectForSwap(std::shared_ptr<Tile> tile) {
	if (tile->getStatus() != Tile::Status::SELECTED_FOR_SWAP) {
		tile->setStatus(Tile::Status::SELECTED_FOR_SWAP);
		tile->getSprite()->setColor(sf::Color(255, 255, 255, 255));
	}
	else {
		tile->setStatus(Tile::Status::IN_HAND);
		tile->getSprite()->setColor(sf::Color(255, 255, 255, 127));
	}
}

std::pair<bool, int> Player::endSwap(std::shared_ptr<Bag> bag) {
	//najpierw sprawdŸ czy w worku jest wystarczaj¹ca iloœæ p³ytek
	int sum = 0, bagSize=bag->size();
	for (std::vector<std::shared_ptr<Tile>>::iterator it = hand.begin(); it != hand.end(); it++) {
		if ((*it)->getStatus() == Tile::Status::SELECTED_FOR_SWAP) sum++;
		(*it)->getSprite()->setColor(sf::Color(255, 255, 255, 255));
	}

	//je¿eli w worku jest mniej p³ytek ni¿ gracz chcia³by wymieniæ / gracz zdecydowa³ siê nie wymieniaæ ¿adych p³ytek
	if (sum > bagSize || sum == 0) return std::make_pair(false,sum);

	for (unsigned int i = 0; i < hand.size(); i++) {
		if (hand[i]->getStatus() == Tile::Status::SELECTED_FOR_SWAP) {
			auto temp = hand[i];
			hand[i] = std::static_pointer_cast<Tile>(bag->grab());
			temp->setStatus(Tile::Status::IN_BAG);
			bag->putBack(temp);
		}
		hand[i]->setStatus(Tile::Status::IN_HAND);
		hand[i]->setCoords(sf::Vector2f(900 + 91.5 * i, 260));
	}
	return std::make_pair(true,sum);
}

void Player::play(std::shared_ptr<Board> board) {
	for (std::vector<std::shared_ptr<Tile>>::iterator it = hand.begin(); it != hand.end(); it++) {
		if ((*it)->getStatus() == Tile::Status::TEMP_ON_BOARD) {
			(*it)->setStatus(Tile::Status::PERM_ON_BOARD);
			board->tilesOnBoard.push_back((*it));
			*it = nullptr;
		}
	}
	hand.erase(std::remove(begin(hand), end(hand), nullptr), end(hand));
}

void Player::pass(std::shared_ptr<Board> board) {
	for (unsigned int i = 0; i < hand.size(); i++) {
		if (hand[i]->getStatus()==Tile::Status::TEMP_ON_BOARD) {
			board->findSlotOnPosition(hand[i]->getPositionOnBoard())->leaveSlot();
			hand[i]->setCoords(sf::Vector2f(900 + 91.5 * i, 260));
			hand[i]->setStatus(Tile::Status::IN_HAND);
			hand[i]->setPositionOnBoard(1);
		}
	}
}