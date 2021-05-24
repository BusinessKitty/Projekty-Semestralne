#pragma once

//! Klasa zawieraj�ca informacje o tekstach renderowanych przez bibliotek� SFML
class Text {
private:
	//! tekst przechowywany przez obiekt
	sf::Text text;
public:
	/*!
		G��wny konstruktor obiektu z paramerem std::string
		\param string ci�g znak�w przechowywany przez obiekt
		\param font wska�nik na czcionk�
		\param size wielko�� liter
		\param position pozycja pocz�tkowa tekstu
		\param setOriginToMiddle informacja czy tekst ma by� renderowany od �rodka
	*/
	Text(std::string string, sf::Font* font, int size, sf::Vector2f position, bool setOriginToMiddle);
	/*!
		G��wny konstruktor obiektu z parametrem std::wstring
		\param wstring ci�g znak�w przechowywany przez obiekt
		\param font wska�nik na czcionk�
		\param size wielko�� liter
		\param position pozycja pocz�tkowa tekstu
		\param setOriginToMiddle informacja czy tekst ma by� renderowany od �rodka
	*/
	Text(std::wstring wstring, sf::Font* font, int size, sf::Vector2f position, bool setOriginToMiddle);
	//! Konstruktor bezargumentowy
	Text();
	//! Destruktor
	~Text();
	/*!
		Funkcja ci�g znak�w na podany
		\param string ci�g znak�w
	*/
	void setString(const std::string& string);
	/*!
		Funkcja ci�g znak�w na podany
		\param wstring ci�g znak�w
	*/
	void setString(const std::wstring& wstring);
	/*!
		Funkcja zwracaj�ca wska�nik na tekst
		\return wska�nik na tekst
	*/
	sf::Text* getText();
	/*!
		Funkcja ustawiaj�ca pozycj�
		\param position pozycja
	*/
	void setPosition(sf::Vector2f position);
	/*!
		Funkcja ustawiaj�ca pozycj� od kt�rej renderowa� na �rodek
	*/
	void setOriginToMiddle();
};