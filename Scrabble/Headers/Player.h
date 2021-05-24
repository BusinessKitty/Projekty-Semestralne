#pragma once

class Board;
class Bag;
//! Klasa przedstawiaj�ca gracza
class Player {
private:
	//! zmienna pomocnicza zawieraj�ca pozycj� z kt�rej podniesiono aktualnie podniesion� p�ytk�
	int grabbedFrom;
public:
	//! identyfikator gracza
	int id;
	//! aktualne punkty gracza
	int points;
	//! wektor p�ytek na r�ce gracza
	std::vector<std::shared_ptr<Tile>> hand;
	//! pozosta�y czas gracza
	sf::Time timeLeft;
	/*!
		G��wny konstruktor obiektu
		\param id_ identyfikator gracza
		\param timeLeft_ czas na gr�
	*/
	Player(int id_,int timeLeft_);
	//! Destruktor
	~Player();
	/*!
		Funkcja rozpoczynaj�ca tur� gracza
		\param bag wska�nik na worek z p�ytkami
	*/
	void startTurn(std::shared_ptr<Bag> bag);
	/*!
		Funkcja odpowiadaj�ca za podniesienie p�ytki
		\param tile p�ytka kt�r� nale�y podnie��
	*/
	void grab(std::shared_ptr<Tile> tile);
	/*!
		Funkcja odpowiadaj�ca za upuszczenie p�ytki
		\param board wzka�nik na plansz�
		\param tile p�ytka kt�r� nale�y upu�ci�
	*/
	void drop(std::shared_ptr<Board> board, std::shared_ptr<Tile> tile);
	/*!
		Funkcja odpowiadaj�ca za zwr�cenie p�ytki z powrotem na r�k� za pomoc� prawego przycisku myszki
		\param tile p�ytka kt�r� nale�y zwr�ci� z powrotem na r�k�
	*/
	void backToHand(std::shared_ptr<Tile> tile);
	/*!
		Funkcja odpowiadaj�ca za zagranie
		\param board wzka�nik na plansz�
	*/
	void play(std::shared_ptr<Board> board);
	/*!
		Funkcja odpowiadaj�ca za rozpocz�cie wymiany
		\param board wzka�nik na plansz�
		\param bag wska�nik na worek z p�ytkami
	*/
	void startSwap(std::shared_ptr<Board> board, std::shared_ptr<Bag> bag);
	/*!
		Funkcja odpowiadaj�ca za zaznaczenie p�ytki do wymiany
		\param tile p�ytka kt�r� nale�y zaznaczy� do wymiany
	*/
	void selectForSwap(std::shared_ptr<Tile> tile);
	/*!
		Funkcja odpowiadaj�ca za zako�czenie wymiany
		\param bag wska�nik na worek z p�ytkami
		\return para: bool - czy nast�pi�a wymiana, int - ile p�ytek nale�a�o wymieni�
	*/
	std::pair<bool,int> endSwap(std::shared_ptr<Bag> bag);
	/*!
		Funkcja odpowiadaj�ca za spasowanie tury
		\param board wzka�nik na plansz�
	*/
	void pass(std::shared_ptr<Board> board);
};
