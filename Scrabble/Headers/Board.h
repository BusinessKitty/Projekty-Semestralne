#pragma once
class Tile;
class Blank;
//! Klasa bêd¹ca reprezentacj¹ planszy do gry
class Board : public Container{
public:
	//! wektor zawieraj¹cy p³ytki bêd¹ce na sta³e na planszy
	std::vector<std::shared_ptr<Tile>> tilesOnBoard;
	//! Konstruktor bezargumentowy
	Board();
	//! Destruktor
	~Board();
	/*!
		Funkcja zwracaj¹ca miejsce na planszy o podanej pozycji
		\param n pozycja na planszy
		\return wskaŸnik na miejsce
	*/
	std::shared_ptr<Slot> findSlotOnPosition(const int& n);
};
