#pragma once

//! Klasa zawieraj�ca informacje o obiektach renderowanych przez bibliotek� SFML
class Drawable {
protected:
	//! wektor globalnej pozycji obiektu
	sf::Vector2f coords;
	//! tekstura pobierana z pliku
	sf::Texture texture;
	//! sprite tworzony za pomoc� tekstury
	sf::Sprite sprite;
	//! globalne granice obiektu
	sf::FloatRect bounds;

public:
	/*!
		G��wny konstruktor obiektu
		\param coords_ pozycja obiektu
		\param filename nazwa pliku tekstury
	*/
	Drawable(sf::Vector2f coords_, std::string filename);
	//! Konstruktor bezargumentowy
	Drawable();
	//! Destruktor
	~Drawable();
	/*!
		Funkcja ustawiaj�ca sprite obiektu
		\param filename nazwa pliku tekstury
	*/
	void setSprite(const std::string& filename);
	/*!
		Funkcja zwracaj�ca wska�nik na sprite obiektu
		\return wska�nik na sprite obiektu
	*/
	sf::Sprite* getSprite();
	/*!
		Funkcja zwracaj�ca wska�nik na globalne granice obiektu
		\return wska�nik na globalne granice obiektu
	*/
	sf::FloatRect* getBounds();
	/*!
		Funkcja ustawiaj�ca pozycj� obiektu
		\param coords_ pozycja obiektu
	*/
	void setCoords(sf::Vector2f coords_);
	/*!
		Funkcja zwracaj�ca pozycj� obiektu
		\return pozycja obiektu
	*/
	sf::Vector2f getCoords();
};