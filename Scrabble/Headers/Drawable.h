#pragma once

//! Klasa zawierajπca informacje o obiektach renderowanych przez bibliotekÍ SFML
class Drawable {
protected:
	//! wektor globalnej pozycji obiektu
	sf::Vector2f coords;
	//! tekstura pobierana z pliku
	sf::Texture texture;
	//! sprite tworzony za pomocπ tekstury
	sf::Sprite sprite;
	//! globalne granice obiektu
	sf::FloatRect bounds;

public:
	/*!
		G≥Ûwny konstruktor obiektu
		\param coords_ pozycja obiektu
		\param filename nazwa pliku tekstury
	*/
	Drawable(sf::Vector2f coords_, std::string filename);
	//! Konstruktor bezargumentowy
	Drawable();
	//! Destruktor
	~Drawable();
	/*!
		Funkcja ustawiajπca sprite obiektu
		\param filename nazwa pliku tekstury
	*/
	void setSprite(const std::string& filename);
	/*!
		Funkcja zwracajπca wskaünik na sprite obiektu
		\return wskaünik na sprite obiektu
	*/
	sf::Sprite* getSprite();
	/*!
		Funkcja zwracajπca wskaünik na globalne granice obiektu
		\return wskaünik na globalne granice obiektu
	*/
	sf::FloatRect* getBounds();
	/*!
		Funkcja ustawiajπca pozycjÍ obiektu
		\param coords_ pozycja obiektu
	*/
	void setCoords(sf::Vector2f coords_);
	/*!
		Funkcja zwracajπca pozycjÍ obiektu
		\return pozycja obiektu
	*/
	sf::Vector2f getCoords();
};