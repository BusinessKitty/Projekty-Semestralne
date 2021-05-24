#pragma once
#include <thread>
#include <set>
class Slot;
class Tile;
class Blank;
//! Klasa reprezentuj¹ca Arbitera gry
class Arbiter : public Singleton<Arbiter> {
	//! zbiór zawieraj¹cy s³owa zaczynaj¹ce siê na litery A-F
	std::set<std::string> af;
	//! zbiór zawieraj¹cy s³owa zaczynaj¹ce siê na litery G-M
	std::set<std::string> gm;
	//! zbiór zawieraj¹cy s³owa zaczynaj¹ce siê na literê N
	std::set<std::string> n;
	//! zbiór zawieraj¹cy s³owa zaczynaj¹ce siê na litery O-P
	std::set<std::string> op;
	//! zbiór zawieraj¹cy s³owa zaczynaj¹ce siê na litery R-U
	std::set<std::string> ru;
	//! zbiór zawieraj¹cy s³owa zaczynaj¹ce siê na litery W-X
	std::set<std::string> wx;

	/*!
		Funkcja ³aduj¹ca s³owa z odpowiedniego pliku do odpowiedniego zbioru (0 - af, 1 - gm, ...)
		\param index indeks zbioru
		\param filename nazwa pliku
	*/
	void load(const int& index, const std::string& filename);
	/*!
		Funkcja sprawdzaj¹ca czy dane zagranie gracza jest prawid³owe
		\param playerHand p³ytki gracza, którego zagranie jest sprawdzane
		\param tilesOnBoard p³ytki bêd¹ce na stale na planszy
		\param text wskaŸnik na tekst wyœwietlaj¹cy informacje
		\return mapa zawieraj¹ca p³ytki które chce zagraæ gracz, oraz przyleg³e im p³ytki z planszy posortowane wg. pozycji na planszy 
	*/
	std::map<int, std::shared_ptr<Tile>> checkPlay(std::vector<std::shared_ptr<Tile>> playerHand, std::vector<std::shared_ptr<Tile>> tilesOnBoard, Text* text);
	/*!
		Funkcja tworz¹ca wektor map<int,std::shared_ptr<Tile>>>; element wektora to s³owo: int to pozycja litery a std::shared_ptr<Tile>> to litera
		\param positions mapa zawieraj¹ca p³ytki które chce zagraæ gracz, oraz przyleg³e im p³ytki z planszy posortowane wg. pozycji na planszy 
		\return utworzony wektor s³ów
	*/
	std::vector<std::map<int, std::shared_ptr<Tile>>> groupLettersAtPositions(std::map<int, std::shared_ptr<Tile>> positions);
	/*!
		Funkcja sprawdzaj¹ca istnienie danego s³owa w s³owniku
		\param word s³owo, którego poprawnoœæ nale¿y sprawdziæ
		\return true - je¿eli s³owo jest poprawne
	*/
	bool isCorrect(const std::string& word);
public:
	//! zmienna pomocnicza przechowuj¹ca informacjê ile zbiorów zosta³o za³adowanych
	int setsLoaded;
	//! G³ówny konstruktor
	Arbiter(token);
	//! Destruktor
	~Arbiter();
	/*!
		Funkcja odpowiadaj¹ca za za³adowanie wszystkich zbiorów
	*/
	void loadSets();
	/*!
		Funkcja zamieniaj¹ca znaki a,e,o,s,l,z,x,n na ¥,Ê,Ó,£,¯,,Ñ w podanym s³owie
		\param s³owo w którym zamieniæ znaki
		\return oczekiwany wstring z polskimi literami
	*/
	std::wstring toPolish(const std::string& word);
	/*!
		\param playerHand p³ytki gracza, którego zagranie jest sprawdzane
		\param board wskaŸnik na planszê
		\param text wskaŸnik na tekst wyœwietlaj¹cy informacje
		\return para: wektor s³ów tworzonych przez gracza oraz sumaryczna iloœæ punktów jak¹ za nie zdobêdzie
	*/
	std::pair<std::vector<std::string>, int> checkWords(std::vector<std::shared_ptr<Tile>> playerHand, std::shared_ptr<Board> board, Text* text);
};