#pragma once

//! Klasa przedstawiaj�ca p�ytk� z liter�
class Tile : public Drawable{
public:
	//! enum okre�laj�cy mo�liwe statusy p�ytek
	enum class Status{
		IN_BAG = 0,			/*!< p�ytka znajduje si� w worku */
		IN_HAND,			/*!< p�ytka znajduje si� w r�ce gracza */
		PICKED_UP,			/*!< p�ytka jest w�a�nie trzymana przez gracza za pomoc� myszki */
		SELECTED_FOR_SWAP,	/*!< p�ytka jest zaznaczona do wymiany */
		TEMP_ON_BOARD,		/*!< p�ytka jest tymczasowo na planszy (tura gracza nie sko�czy�a si�) */
		PERM_ON_BOARD		/*!< p�ytka jest trwale na planszy */
	};
protected:
	//! aktualny status danej p�ytki
	Tile::Status status;
	//! litera na danej p�ytce
	char letter;
	//! punkty za dan� liter�
	int points;
	//! pozycja p�ytki na planszy (od -1 do 224) - je�eli (-1) to p�ytka nie jest aktualnie na planszy
	int positionOnBoard;
public:
	/*!
		G��wny konstruktor obiektu
		\param coords_ Wektor globalnej pozycji obiektu
		\param filename nazwa pliku tekstury
		\param letter_ litera na danej p�ytce
		\param points_ punkty za dan� p�ytk�
	*/
	Tile(sf::Vector2f coords_, std::string filename, char letter_,int points_);
	//! Destruktor
	~Tile();
	/*!
		Funkcja ustawiaj�ca status p�ytki
		\param status_ status p�ytki
	*/
	void setStatus(const Tile::Status& status_);
	/*!
		Funkcja zwracaj�ca status p�ytki
		\return status p�ytki
	*/
	Tile::Status getStatus();
	/*!
		Funkcja zwracaj�ca liter� na danej p�ytce
		\return litera na danej p�ytce
	*/
	char getLetter();
	/*!
		Funkcja zwracaj�ca punkty za dan� p�ytk�
		\return punkty za dan� p�ytk�
	*/
	int getPoints();
	/*!
		Funkcja ustawiaj�ca pozycj� p�ytki na planszy
		\param offBoard je�eli true - ustaw pozycj� na -1, je�eli false - ustaw pozycj� wg coords
	*/
	void setPositionOnBoard(const bool& offBoard=0);
	/*!
		Funkcja zwracaj�ca pozycj� p�ytki na planszy
		\return pozycja p�ytki na planszy
	*/
	int getPositionOnBoard();
	/*!
		Operator por�wnania pozycji na planszy dw�ch p�yek
	*/
	bool operator==(const Tile& lhs) { return this->positionOnBoard == lhs.positionOnBoard; }
};
