#pragma once
class Drawable;
//! Klasa przedstawiaj�ca "pojemnik" na wska�niki na obiekty typu Drawable
class Container {
protected:
	//! wektor zawieraj�cy wska�niki na obiekty Drawable
	std::vector<std::shared_ptr<Drawable>> contents;
public:
	//! Konstruktor bezargumetowy
	Container();
	//! Destruktor
	~Container();
	/*!
		Funkcja zwracaj�ca rozmiar pojemnika
		\return rozmiar pojemnika
	*/
	int size();
	/*!
		Operator dost�pu do obiektu na podanym miejscu
	*/
	std::shared_ptr<Drawable> operator[](const int& n) {
		return contents[n];
	}
	/*!
		Funkcja zwracaj�ca iterator na pocz�tek pojemnika
		\return iterator na pocz�tek pojemnika
	*/
	std::vector<std::shared_ptr<Drawable>>::iterator begin();
	/*!
		Funkcja zwracaj�ca iterator na koniec pojemnika
		\return iterator na koniec pojemnika
	*/
	std::vector<std::shared_ptr<Drawable>>::iterator end();
};