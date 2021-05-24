#pragma warning(push, 0)
#include <SFML/Graphics.hpp>
#pragma warning(pop)

#include <fstream>
#include <thread>
#include <set>
#include <codecvt>
#include <memory>

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


Arbiter::Arbiter(token) : setsLoaded(0) {}
Arbiter::~Arbiter(){}

void Arbiter::load(const int& index,const std::string& filename) {
	std::string word;
	std::ifstream file(filename);
	if (index == 0) {
		if (file.is_open()) {
			while (std::getline(file, word)) {
				af.insert(word);
			}
			file.close();
			setsLoaded++;
		}
	}
	else if (index == 1) {
		if (file.is_open()) {
			while (std::getline(file, word)) {
				gm.insert(word);
			}
			file.close();
			setsLoaded++;
		}
	}
	else if (index == 2) {
		if (file.is_open()) {
			while (std::getline(file, word)) {
				n.insert(word);
			}
			file.close();
			setsLoaded++;
		}
	}
	else if (index == 3) {
		if (file.is_open()) {
			while (std::getline(file, word)) {
				op.insert(word);
			}
			file.close();
			setsLoaded++;
		}
	}
	else if (index == 4) {
		if (file.is_open()) {
			while (std::getline(file, word)) {
				ru.insert(word);
			}
			file.close();
			setsLoaded++;
		}
	}
	else if (index == 5) {
		if (file.is_open()) {
			while (std::getline(file, word)) {
				wx.insert(word);
			}
			file.close();
			setsLoaded++;
		}
	}
}

void Arbiter::loadSets() {
	std::thread([=] {load(0, "Resources/dictionaries/af.txt"); }).detach();
	std::thread([=] {load(1, "Resources/dictionaries/gm.txt"); }).detach();
	std::thread([=] {load(2, "Resources/dictionaries/n.txt"); }).detach();
	std::thread([=] {load(3, "Resources/dictionaries/op.txt"); }).detach();
	std::thread([=] {load(4, "Resources/dictionaries/ru.txt"); }).detach();
	std::thread([=] {load(5, "Resources/dictionaries/wkoniec.txt"); }).detach();
}

bool Arbiter::isCorrect(const std::string& word) {
	//jeżeli słowo zaczyna się na A|a|B|C|c|D|E|e|F to szukaj w pierwszym secie
	if (((word[0] >= 'A' && word[0] <= 'F') || (word[0] == 'a' || word[0] == 'e' || word[0] == 'c')) && af.find(word) != af.end()) return 1;
	//jeżeli słowo zaczyna się na G|H|I|J|K|L|l|M to szukaj w drugim secie
	else if (((word[0] >= 'G' && word[0] <= 'M') || (word[0] == 'l')) && gm.find(word) != gm.end()) return 1;
	//jeżeli słowo zaczyna się na N to szukaj w trzecim secie (nie ma słów zaczynających się na Ń)
	else if (word[0] == 'N' && n.find(word) != n.end()) return 1;
	//jeżeli słowo zaczyna się na O|o|P to szukaj w czwartym secie
	else if (((word[0] >= 'O' && word[0] <= 'P') || (word[0] == 'o')) && op.find(word) != op.end()) return 1;
	//jeżeli słowo zaczyna się na R|S|s|T|U to szukaj w piątym secie
	else if (((word[0] >= 'R' && word[0] <= 'U') || (word[0] == 's')) && ru.find(word) != ru.end()) return 1;
	//jeżeli słowo zaczyna się na W|Z|z|x|Y
	else if (wx.find(word) != wx.end()) return 1;
	else return 0;
}

std::pair<std::vector<std::string>, int> Arbiter::checkWords(std::vector<std::shared_ptr<Tile>> playerHand, std::shared_ptr<Board> board, Text* text) {

	std::map<int, std::shared_ptr<Tile>> positions = checkPlay(playerHand, board->tilesOnBoard, text);
	if (positions.empty()) return std::make_pair(std::vector<std::string>(), 0);

	std::vector<std::map<int, std::shared_ptr<Tile>>> grouped = groupLettersAtPositions(positions);

	std::vector<std::string> words;
	std::vector<int> vals;
	int sum=0,wordval=0,letterval=0,wordmult=1;
	std::string word;
	//dla każdego elementu w grouped
	for (std::vector<std::map<int, std::shared_ptr<Tile>>>::iterator it = grouped.begin(); it != grouped.end(); it++) {
		//dla każdego elementu w mapie
		for (std::map<int, std::shared_ptr<Tile>>::iterator itt = (*it).begin(); itt != (*it).end(); itt++) {
			//zapisz wartość litery
			letterval = (*itt).second->getPoints();
			//jeśli to litera położona przez gracza to znajdź multiplier
			if ((*itt).second->getStatus() == Tile::Status::TEMP_ON_BOARD) {
				for (std::vector<std::shared_ptr<Drawable>>::iterator ittt = board->begin(); ittt != board->end(); ittt++) {
					std::shared_ptr<Slot> slot = std::static_pointer_cast<Slot>(*ittt);
					if (slot->getPositionOnBoard() == (*itt).first) {
						std::pair<int, Slot::MultiplierType> multiplier = slot->getMultiplier();
						//jeśli to multiplier litery pomnóż letterval razy multiplier
						if (multiplier.second == Slot::MultiplierType::LETTER) letterval *= multiplier.first;
						//jeśli to multiplier słowa ustaw multiplier do wordmult
						else wordmult = multiplier.first;

						break;
					}
				}
			}
			//dodaj literę do tworzonego słowa
			if ((*itt).second->getLetter() == '_') {
				word.push_back(std::static_pointer_cast<Blank>((*itt).second)->getPicked());
			}else word.push_back((*itt).second->getLetter());
			//dodaj do wartości słowa wartość litery
			wordval += letterval;
			//reset dla nastepnej litery...
			letterval = 0;
		}
		//dodaj słowo do wektora
		words.push_back(word);
		//pomnóż wartość słowa przez wordmult
		wordval *= wordmult;
		//dodaj wartość słowa do sumy 
		sum += wordval;
		vals.push_back(wordval);
		//reset dla nastepnego słowa...
		wordval = 0;
		wordmult = 1;
		word.clear();
	}
	for (std::vector<std::string>::iterator it = words.begin(); it != words.end();it++) {
		if (!isCorrect((*it))) {
			std::wstring message = L"Słowo ";
			std::wstring temp = toPolish((*it));
			message.append(temp);
			message.append(L" nie istnieje w słowniku");
			text->setString(message);
			text->setPosition(sf::Vector2f(450, 45));
			text->setOriginToMiddle();
			return std::make_pair(std::vector<std::string>(), 0);
		}
	}
	std::wstring message;
	for (unsigned int i = 0; i < words.size();i++) {
		std::wstring temp = toPolish(words[i]);
		message.append(temp);
		message.append(L": ");
		temp = toPolish(std::to_string(vals[i]));
		message.append(temp);
		if(i!=words.size()-1)message.append(L", ");
	}
	text->setString(message);
	text->setPosition(sf::Vector2f(450, 45));
	text->setOriginToMiddle();
	return std::make_pair(words, sum);
}

std::map<int, std::shared_ptr<Tile>> Arbiter::checkPlay(std::vector<std::shared_ptr<Tile>> playerHand, std::vector<std::shared_ptr<Tile>> tilesOnBoard, Text* text) {

	//std::map<int,Tile*> played - płytki zagrane przez gracza posortowane wg pozycji
	std::map<int, std::shared_ptr<Tile>> played;
	for (std::vector<std::shared_ptr<Tile>>::iterator it = playerHand.begin(); it != playerHand.end(); it++) {
		if ((*it)->getStatus() == Tile::Status::TEMP_ON_BOARD) {
			played[(*it)->getPositionOnBoard()] = (*it);
		}
	}

	//********* CONAJMNIEJ JEDNA PŁYTKA MUSI ZOSTAĆ ZAGRANA ***************//
	if (played.size() == 0) {
		text->setString(L"Przeciągnij płytkę na planszę, aby zagrać");
		text->setPosition(sf::Vector2f(450, 45));
		text->setOriginToMiddle();
		return std::map<int, std::shared_ptr<Tile>>();
	}
	//*********************************************************************//

	//**** PŁYKI MUSZĄ BYĆ ZAGRANE W JEDNEJ LINI (pionowo lub poziomo) ****//
	//zapisz x i y pierwszej płytki
	int x = (*played.begin()).first % 15;
	int y = (*played.begin()).first / 15;
	//każda położona płytka musi mieć albo ten sam x albo ten sam y
	bool horizontal = 1, vertical = 1;
	for (std::map<int, std::shared_ptr<Tile>>::iterator it = played.begin(); it != played.end(); it++) {
		if ((*it).first % 15 != x) {
			vertical = 0;
			break;
		}
	}
	for (std::map<int, std::shared_ptr<Tile>>::iterator it = played.begin(); it != played.end(); it++) {
		if ((*it).first / 15 != y) {
			horizontal = 0;
			break;
		}
	}
	//jeżeli gracz nie układa słowa ani pionowo ani poziomo
	if ((!horizontal && !vertical)) {
		text->setString(L"Płytki nie są ułożone w jednej lini");
		text->setPosition(sf::Vector2f(450, 45));
		text->setOriginToMiddle();
		return std::map<int, std::shared_ptr<Tile>>();
	}
	//*********************************************************************//

	//jeżeli na planszy nie ma żadnych płyek to znaczy że nadal trwa "pierwsza tura"
	if (tilesOnBoard.empty()) {
		//*************** SŁOWA JEDNOLITEROWE NIEDOZWOLONE ********************//
		if (played.size() == 1) {
			text->setString(L"Słowa jednoliterowe są niedozwolone");
			text->setPosition(sf::Vector2f(450, 45));
			text->setOriginToMiddle();
			return std::map<int, std::shared_ptr<Tile>>();
		}
		//*********************************************************************//
		//* GRACZ ROZPOCZYNAJĄCY MUSI SKORZYSTAĆ Z ŚRODKOWEGO POLA (112) **//

		if (played.find(112) == played.end()) {
			text->setString(L"Gracz rozpoczynający musi skorzystać z środkowego pola");
			text->setPosition(sf::Vector2f(450, 45));
			text->setOriginToMiddle();
			return std::map<int, std::shared_ptr<Tile>>();
		}
		//******** PŁYTKI GRACZA ROZPOCZYNAJĄCEGO MUSZĄ SIĘ DOTYKAĆ *******//
		if (horizontal) {
			for (std::map<int, std::shared_ptr<Tile>>::iterator it = played.begin(); it != played.end(); it++) {
				if (std::next(it) != played.end()) {
					if ((*std::next(it)).first != (*it).first + 1) {
						text->setString(L"Płytki nie tworzą jednego słowa");
						text->setPosition(sf::Vector2f(450, 45));
						text->setOriginToMiddle();
						return std::map<int, std::shared_ptr<Tile>>();
					}
				}
			}
		}
		else {
			for (std::map<int, std::shared_ptr<Tile>>::iterator it = played.begin(); it != played.end(); it++) {
				if (std::next(it) != played.end()) {
					if ((*std::next(it)).first != (*it).first + 15) {
						text->setString(L"Płytki nie tworzą jednego słowa");
						text->setPosition(sf::Vector2f(450, 45));
						text->setOriginToMiddle();
						return std::map<int, std::shared_ptr<Tile>>();
					}
				}
			}
		}

		//*****************************************************************//
		return played;
	}
	//*********************************************************************//

	//posortuj płytki na planszy wg pozycji std::map<int, Tile*> board
	std::map<int, std::shared_ptr<Tile>> board;
	for (std::vector<std::shared_ptr<Tile>>::iterator it = tilesOnBoard.begin(); it != tilesOnBoard.end(); it++) {
		board[(*it)->getPositionOnBoard()] = (*it);
	}
	//dla każdej płytki w played sprawdzaj płytki w board w każdą stronę i dodawaj do setu result
	std::map<int, std::shared_ptr<Tile>> result;
	std::map<int, std::shared_ptr<Tile>>::iterator itt;
	for (std::map<int, std::shared_ptr<Tile>>::iterator it = played.begin(); it != played.end(); it++) {
		int i = 1;
		//sprawdzaj na lewo do momentu trafienia na (*itt).first+1 % 15 == 0
		itt = board.find((*it).first - i);
		if (itt != board.end() && ((*itt).first + 1) % 15 == 0)itt = board.end();
		while (itt != board.end()) {
			result[(*itt).first] = (*itt).second;
			i++;
			itt = board.find((*it).first - i);
			if (itt != board.end() && ((*itt).first + 1) % 15 == 0)itt = board.end();
		}
		i = 1;
		//sprawdzaj na prawo do momentu trafienia na (*itt).first % 15 == 0
		itt = board.find((*it).first + i);
		if (itt != board.end() && (*itt).first % 15 == 0)itt = board.end();
		while (itt != board.end()) {
			result[(*itt).first] = (*itt).second;
			i++;
			itt = board.find((*it).first + i);
			if (itt != board.end() && (*itt).first % 15 == 0)itt = board.end();
		}
		i = 1;
		//sprawdzaj do góry
		itt = board.find((*it).first - i * 15);
		while (itt != board.end()) {
			result[(*itt).first] = (*itt).second;
			i++;
			itt = board.find((*it).first - i * 15);
		}
		i = 1;
		//sprawdzaj na dół
		itt = board.find((*it).first + i * 15);
		while (itt != board.end()) {
			result[(*itt).first] = (*itt).second;
			i++;
			itt = board.find((*it).first + i * 15);
		}
		i = 1;
	}
	//dodaj płytki z played do result
	for (std::map<int, std::shared_ptr<Tile>>::iterator it = played.begin(); it != played.end(); it++) {
		result[(*it).first] = (*it).second;
	}
	// SŁOWO TWORZONE PRZEZ GRACZA MUSI BYĆ PRZYŁOŻONE DO LITERY NA PLANSZY//
	if (result.size() == played.size()) {
		text->setString(L"Słowo musi zostać utworzone dokładając płytki do obecnych na planszy");
		text->setPosition(sf::Vector2f(450, 45));
		text->setOriginToMiddle();
		return std::map<int, std::shared_ptr<Tile>>();
	}

	//***** KAŻDA ZAGRANA PŁYTKA MUSI MIEĆ CONAJMNIEJ JEDNEGO SĄSIADA *****//
	for (std::map<int, std::shared_ptr<Tile>>::iterator it = played.begin(); it != played.end(); it++) {
		std::map<int, std::shared_ptr<Tile>>::iterator left = result.find((*it).first - 1);
		std::map<int, std::shared_ptr<Tile>>::iterator right = result.find((*it).first + 1);
		std::map<int, std::shared_ptr<Tile>>::iterator up = result.find((*it).first - 15);
		std::map<int, std::shared_ptr<Tile>>::iterator down = result.find((*it).first + 15);
		if (left == result.end() && right == result.end() && up == result.end() && down == result.end()) {
			text->setString(L"Płytki nie tworzą jednego słowa");
			text->setPosition(sf::Vector2f(450, 45));
			text->setOriginToMiddle();
			return std::map<int, std::shared_ptr<Tile>>();
		}
	}
	return result;
}

std::vector<std::map<int, std::shared_ptr<Tile>>> Arbiter::groupLettersAtPositions(std::map<int, std::shared_ptr<Tile>> positions) {
	//zapisz główne słowo
	std::map<int, std::shared_ptr<Tile>> mainWord;
	for (std::map<int, std::shared_ptr<Tile>>::iterator it = positions.begin(); it != positions.end(); it++) {
		if ((*it).second->getStatus() == Tile::Status::TEMP_ON_BOARD) {
			mainWord[(*it).first] = (*it).second;
		}
	}
	
	if (mainWord.size() == 1) {
		std::vector<std::map<int, std::shared_ptr<Tile>>> words;
		std::map<int, std::shared_ptr<Tile>> h, v;
		for (std::map<int, std::shared_ptr<Tile>>::iterator it = positions.begin(); it != positions.end(); it++) {
			if ((*it).first == mainWord.begin()->first) {
				h[(*it).first] = (*it).second;
				v[(*it).first] = (*it).second;
			}
			else if ((*it).first / 15 == mainWord.begin()->first / 15) h[(*it).first] = (*it).second;
			else {
				v[(*it).first] = (*it).second;
			}
		}
		if(h.size()!=1)words.push_back(h);
		if(v.size() != 1)words.push_back(v);
		return words;
	}

	//sprawdź czy gracz układa poziomo czy pionowo
	bool horizontally = 0;
	std::map<int, std::shared_ptr<Tile>>::iterator temp = mainWord.begin();
	//jeżeli dwie dowolne układane przez gracza płytki mają ten sam x (positionOnBoard/15) to gracz układa poziomo
	if (std::next(temp)->first / 15 == temp->first / 15) horizontally = 1;

	//uzupełnij słowo główne
	for (std::map<int, std::shared_ptr<Tile>>::iterator it = positions.begin(); it != positions.end(); it++) {
		if (horizontally) {
			if ((*it).first / 15 == temp->first/15) mainWord[(*it).first] = (*it).second;
		} else {
			if ((*it).first % 15 == temp->first % 15) mainWord[(*it).first] = (*it).second;
		}
	}
	//zapisz słowo główne do wektora words
	std::vector<std::map<int, std::shared_ptr<Tile>>> words;
	std::map<int, std::shared_ptr<Tile>> word;
	for (std::map<int, std::shared_ptr<Tile>>::iterator it = mainWord.begin(); it != mainWord.end(); it++) {
		word[(*it).first]=(*it).second;
	}
	words.push_back(word);

	//dla każdej płytki w słowie głównym znajdź słowo pionowe jeżeli gracz układa poziomo i poziome jeżeli pionowo
	for (std::map<int, std::shared_ptr<Tile>>::iterator it = mainWord.begin(); it != mainWord.end(); it++) {
		word.clear();
		if (horizontally) {
			for (std::map<int, std::shared_ptr<Tile>>::iterator itt = positions.begin(); itt != positions.end(); itt++) {
				if ((*itt).first % 15 == (*it).first % 15)word[(*itt).first]=(*itt).second;
			}
		}
		else {
			for (std::map<int, std::shared_ptr<Tile>>::iterator itt = positions.begin(); itt != positions.end(); itt++) {
				if ((*itt).first / 15 == (*it).first / 15)word[(*itt).first] = (*itt).second;
			}
		}
		if (word.size() != 1)words.push_back(word);
	}

	return words;
}

std::wstring Arbiter::toPolish(const std::string& word) {
	std::wstring result;
	for (unsigned int i = 0; i < word.size(); i++) {
		if (word[i] >= 65 && word[i] <= 90)result.push_back(word[i]);
		else {
			if (word[i] == 'a') result.push_back(L'Ą');
			else if (word[i] == 'e') result.push_back(L'Ę');
			else if (word[i] == 'o') result.push_back(L'Ó');
			else if (word[i] == 's') result.push_back(L'Ś');
			else if (word[i] == 'l') result.push_back(L'Ł');
			else if (word[i] == 'z') result.push_back(L'Ż');
			else if (word[i] == 'x') result.push_back(L'Ź');
			else if (word[i] == 'c') result.push_back(L'Ć');
			else if (word[i] == 'n') result.push_back(L'Ń');
			else result.push_back(word[i]);
		}
	}
	return result;
}