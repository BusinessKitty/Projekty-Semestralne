#pragma once
class Blank : public Tile {
private:
	//! litera wybrana na miejsce pustej p�ytki
	char picked;
public:
	/*!
		G��wny konstruktor obiektu
		\param coords_ Wektor globalnej pozycji obiektu
		\param filename nazwa pliku tekstury
		\param letter_ litera na danej p�ytce
		\param points_ punkty za dan� p�ytk�
	*/
	Blank(sf::Vector2f coords_, std::string filename, char letter_, int points_);
	//! Destruktor
	~Blank();
	/*!
		Funkcja ustawiaj�ca liter� na miejsce pustej p�ytki
		\param picked_ litera wybrana na miejsce pustej p�ytki
	*/
	void setPicked(const char& picked_);
	/*!
		Funkcja zwracaj�ca wybran� liter�
		\return litera wybrana na miejsce pustej p�ytki
	*/
	char getPicked();
};
#pragma once
