#pragma once
#include <thread>
#include <set>
class Slot;
class Tile;
class Blank;
//! Klasa reprezentuj�ca Arbitera gry
class Arbiter : public Singleton<Arbiter> {
	//! zbi�r zawieraj�cy s�owa zaczynaj�ce si� na litery A-F
	std::set<std::string> af;
	//! zbi�r zawieraj�cy s�owa zaczynaj�ce si� na litery G-M
	std::set<std::string> gm;
	//! zbi�r zawieraj�cy s�owa zaczynaj�ce si� na liter� N
	std::set<std::string> n;
	//! zbi�r zawieraj�cy s�owa zaczynaj�ce si� na litery O-P
	std::set<std::string> op;
	//! zbi�r zawieraj�cy s�owa zaczynaj�ce si� na litery R-U
	std::set<std::string> ru;
	//! zbi�r zawieraj�cy s�owa zaczynaj�ce si� na litery W-X
	std::set<std::string> wx;

	/*!
		Funkcja �aduj�ca s�owa z odpowiedniego pliku do odpowiedniego zbioru (0 - af, 1 - gm, ...)
		\param index indeks zbioru
		\param filename nazwa pliku
	*/
	void load(const int& index, const std::string& filename);
	/*!
		Funkcja sprawdzaj�ca czy dane zagranie gracza jest prawid�owe
		\param playerHand p�ytki gracza, kt�rego zagranie jest sprawdzane
		\param tilesOnBoard p�ytki b�d�ce na stale na planszy
		\param text wska�nik na tekst wy�wietlaj�cy informacje
		\return mapa zawieraj�ca p�ytki kt�re chce zagra� gracz, oraz przyleg�e im p�ytki z planszy posortowane wg. pozycji na planszy 
	*/
	std::map<int, std::shared_ptr<Tile>> checkPlay(std::vector<std::shared_ptr<Tile>> playerHand, std::vector<std::shared_ptr<Tile>> tilesOnBoard, Text* text);
	/*!
		Funkcja tworz�ca wektor map<int,std::shared_ptr<Tile>>>; element wektora to s�owo: int to pozycja litery a std::shared_ptr<Tile>> to litera
		\param positions mapa zawieraj�ca p�ytki kt�re chce zagra� gracz, oraz przyleg�e im p�ytki z planszy posortowane wg. pozycji na planszy 
		\return utworzony wektor s��w
	*/
	std::vector<std::map<int, std::shared_ptr<Tile>>> groupLettersAtPositions(std::map<int, std::shared_ptr<Tile>> positions);
	/*!
		Funkcja sprawdzaj�ca istnienie danego s�owa w s�owniku
		\param word s�owo, kt�rego poprawno�� nale�y sprawdzi�
		\return true - je�eli s�owo jest poprawne
	*/
	bool isCorrect(const std::string& word);
public:
	//! zmienna pomocnicza przechowuj�ca informacj� ile zbior�w zosta�o za�adowanych
	int setsLoaded;
	//! G��wny konstruktor
	Arbiter(token);
	//! Destruktor
	~Arbiter();
	/*!
		Funkcja odpowiadaj�ca za za�adowanie wszystkich zbior�w
	*/
	void loadSets();
	/*!
		Funkcja zamieniaj�ca znaki a,e,o,s,l,z,x,n na �,�,�,�,�,�,� w podanym s�owie
		\param s�owo w kt�rym zamieni� znaki
		\return oczekiwany wstring z polskimi literami
	*/
	std::wstring toPolish(const std::string& word);
	/*!
		\param playerHand p�ytki gracza, kt�rego zagranie jest sprawdzane
		\param board wska�nik na plansz�
		\param text wska�nik na tekst wy�wietlaj�cy informacje
		\return para: wektor s��w tworzonych przez gracza oraz sumaryczna ilo�� punkt�w jak� za nie zdob�dzie
	*/
	std::pair<std::vector<std::string>, int> checkWords(std::vector<std::shared_ptr<Tile>> playerHand, std::shared_ptr<Board> board, Text* text);
};