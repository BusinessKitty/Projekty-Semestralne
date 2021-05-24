#pragma once

//! Klasa przedstawiająca jedno miejsce na planszy
class Slot : public Drawable {
public:
	//! enum określający możliwe typy mnożników punktowych Slotów
	enum class MultiplierType{
		LETTER = 0, /*!< mnożnik słowny   */
		WORD		/*!< mnożnik literowy */
	};
private:
	//! wartość i typ mnożnika punktowego
	std::pair<int, Slot::MultiplierType> multiplier;
	//! informacja o tym, czy na danym miejscu znajduje się płytka
	bool taken;
	//! pozycja miejsca na planszy (od 0 do 224)
	int positionOnBoard;
public:
	/*!
		Główny konstruktor obiektu
		\param coords_ Wektor globalnej pozycji obiektu
		\param filename nazwa pliku tekstury
		\param multiplier_ wartość i typ mnożnika punktowego
	*/
	Slot(sf::Vector2f coords_, std::string filename, std::pair<int,Slot::MultiplierType> multiplier_);
	//! Konstruktor bezargumentowy
	Slot();
	//! Destruktor
	~Slot();
	/*!
		Funkcja zwracająca wartość i typ mnożnika punktowego
		\return wartość i typ mnożnika punktowego
	*/
	std::pair<int, Slot::MultiplierType> getMultiplier();
	/*!
		Funkcja zwracająca pozycję miejsca na planszy
		\return pozycja miejsca na planszy
	*/
	int getPositionOnBoard();
	/*!
		Funkcja używana, gdy na dane miejsce zostanie położona płytka
	*/
	void takeSlot();
	/*!
		Funkcja używana, gdy z danego miejsca zostanie zabrana płytka
	*/
	void leaveSlot();
	/*!
		Funkcja zwracająca informację o tym, czy na danym miejscu znajduje się płytka
		\return informacja o tym, czy na danym miejscu znajduje się płytka
	*/
	bool isTaken();
};

