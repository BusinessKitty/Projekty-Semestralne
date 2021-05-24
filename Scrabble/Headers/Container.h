#pragma once
class Drawable;
//! Klasa przedstawiajπca "pojemnik" na wskaüniki na obiekty typu Drawable
class Container {
protected:
	//! wektor zawierajπcy wskaüniki na obiekty Drawable
	std::vector<std::shared_ptr<Drawable>> contents;
public:
	//! Konstruktor bezargumetowy
	Container();
	//! Destruktor
	~Container();
	/*!
		Funkcja zwracajπca rozmiar pojemnika
		\return rozmiar pojemnika
	*/
	int size();
	/*!
		Operator dostÍpu do obiektu na podanym miejscu
	*/
	std::shared_ptr<Drawable> operator[](const int& n) {
		return contents[n];
	}
	/*!
		Funkcja zwracajπca iterator na poczπtek pojemnika
		\return iterator na poczπtek pojemnika
	*/
	std::vector<std::shared_ptr<Drawable>>::iterator begin();
	/*!
		Funkcja zwracajπca iterator na koniec pojemnika
		\return iterator na koniec pojemnika
	*/
	std::vector<std::shared_ptr<Drawable>>::iterator end();
};