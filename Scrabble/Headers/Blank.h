#pragma once
class Blank : public Tile {
private:
	//! litera wybrana na miejsce pustej p³ytki
	char picked;
public:
	/*!
		G³ówny konstruktor obiektu
		\param coords_ Wektor globalnej pozycji obiektu
		\param filename nazwa pliku tekstury
		\param letter_ litera na danej p³ytce
		\param points_ punkty za dan¹ p³ytkê
	*/
	Blank(sf::Vector2f coords_, std::string filename, char letter_, int points_);
	//! Destruktor
	~Blank();
	/*!
		Funkcja ustawiaj¹ca literê na miejsce pustej p³ytki
		\param picked_ litera wybrana na miejsce pustej p³ytki
	*/
	void setPicked(const char& picked_);
	/*!
		Funkcja zwracaj¹ca wybran¹ literê
		\return litera wybrana na miejsce pustej p³ytki
	*/
	char getPicked();
};
#pragma once
