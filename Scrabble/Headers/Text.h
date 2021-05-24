#pragma once

//! Klasa zawieraj¹ca informacje o tekstach renderowanych przez bibliotekê SFML
class Text {
private:
	//! tekst przechowywany przez obiekt
	sf::Text text;
public:
	/*!
		G³ówny konstruktor obiektu z paramerem std::string
		\param string ci¹g znaków przechowywany przez obiekt
		\param font wskaŸnik na czcionkê
		\param size wielkoœæ liter
		\param position pozycja pocz¹tkowa tekstu
		\param setOriginToMiddle informacja czy tekst ma byæ renderowany od œrodka
	*/
	Text(std::string string, sf::Font* font, int size, sf::Vector2f position, bool setOriginToMiddle);
	/*!
		G³ówny konstruktor obiektu z parametrem std::wstring
		\param wstring ci¹g znaków przechowywany przez obiekt
		\param font wskaŸnik na czcionkê
		\param size wielkoœæ liter
		\param position pozycja pocz¹tkowa tekstu
		\param setOriginToMiddle informacja czy tekst ma byæ renderowany od œrodka
	*/
	Text(std::wstring wstring, sf::Font* font, int size, sf::Vector2f position, bool setOriginToMiddle);
	//! Konstruktor bezargumentowy
	Text();
	//! Destruktor
	~Text();
	/*!
		Funkcja ci¹g znaków na podany
		\param string ci¹g znaków
	*/
	void setString(const std::string& string);
	/*!
		Funkcja ci¹g znaków na podany
		\param wstring ci¹g znaków
	*/
	void setString(const std::wstring& wstring);
	/*!
		Funkcja zwracaj¹ca wskaŸnik na tekst
		\return wskaŸnik na tekst
	*/
	sf::Text* getText();
	/*!
		Funkcja ustawiaj¹ca pozycjê
		\param position pozycja
	*/
	void setPosition(sf::Vector2f position);
	/*!
		Funkcja ustawiaj¹ca pozycjê od której renderowaæ na œrodek
	*/
	void setOriginToMiddle();
};