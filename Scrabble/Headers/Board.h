#pragma once
class Tile;
class Blank;
//! Klasa b�d�ca reprezentacj� planszy do gry
class Board : public Container{
public:
	//! wektor zawieraj�cy p�ytki b�d�ce na sta�e na planszy
	std::vector<std::shared_ptr<Tile>> tilesOnBoard;
	//! Konstruktor bezargumentowy
	Board();
	//! Destruktor
	~Board();
	/*!
		Funkcja zwracaj�ca miejsce na planszy o podanej pozycji
		\param n pozycja na planszy
		\return wska�nik na miejsce
	*/
	std::shared_ptr<Slot> findSlotOnPosition(const int& n);
};
