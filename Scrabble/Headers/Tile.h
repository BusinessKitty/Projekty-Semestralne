#pragma once

//! Klasa przedstawiaj¹ca p³ytkê z liter¹
class Tile : public Drawable{
public:
	//! enum okreœlaj¹cy mo¿liwe statusy p³ytek
	enum class Status{
		IN_BAG = 0,			/*!< p³ytka znajduje siê w worku */
		IN_HAND,			/*!< p³ytka znajduje siê w rêce gracza */
		PICKED_UP,			/*!< p³ytka jest w³aœnie trzymana przez gracza za pomoc¹ myszki */
		SELECTED_FOR_SWAP,	/*!< p³ytka jest zaznaczona do wymiany */
		TEMP_ON_BOARD,		/*!< p³ytka jest tymczasowo na planszy (tura gracza nie skoñczy³a siê) */
		PERM_ON_BOARD		/*!< p³ytka jest trwale na planszy */
	};
protected:
	//! aktualny status danej p³ytki
	Tile::Status status;
	//! litera na danej p³ytce
	char letter;
	//! punkty za dan¹ literê
	int points;
	//! pozycja p³ytki na planszy (od -1 do 224) - je¿eli (-1) to p³ytka nie jest aktualnie na planszy
	int positionOnBoard;
public:
	/*!
		G³ówny konstruktor obiektu
		\param coords_ Wektor globalnej pozycji obiektu
		\param filename nazwa pliku tekstury
		\param letter_ litera na danej p³ytce
		\param points_ punkty za dan¹ p³ytkê
	*/
	Tile(sf::Vector2f coords_, std::string filename, char letter_,int points_);
	//! Destruktor
	~Tile();
	/*!
		Funkcja ustawiaj¹ca status p³ytki
		\param status_ status p³ytki
	*/
	void setStatus(const Tile::Status& status_);
	/*!
		Funkcja zwracaj¹ca status p³ytki
		\return status p³ytki
	*/
	Tile::Status getStatus();
	/*!
		Funkcja zwracaj¹ca literê na danej p³ytce
		\return litera na danej p³ytce
	*/
	char getLetter();
	/*!
		Funkcja zwracaj¹ca punkty za dan¹ p³ytkê
		\return punkty za dan¹ p³ytkê
	*/
	int getPoints();
	/*!
		Funkcja ustawiaj¹ca pozycjê p³ytki na planszy
		\param offBoard je¿eli true - ustaw pozycjê na -1, je¿eli false - ustaw pozycjê wg coords
	*/
	void setPositionOnBoard(const bool& offBoard=0);
	/*!
		Funkcja zwracaj¹ca pozycjê p³ytki na planszy
		\return pozycja p³ytki na planszy
	*/
	int getPositionOnBoard();
	/*!
		Operator porównania pozycji na planszy dwóch p³yek
	*/
	bool operator==(const Tile& lhs) { return this->positionOnBoard == lhs.positionOnBoard; }
};
