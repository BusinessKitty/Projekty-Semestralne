#pragma once

class Board;
class Bag;
//! Klasa przedstawiaj¹ca gracza
class Player {
private:
	//! zmienna pomocnicza zawieraj¹ca pozycjê z której podniesiono aktualnie podniesion¹ p³ytkê
	int grabbedFrom;
public:
	//! identyfikator gracza
	int id;
	//! aktualne punkty gracza
	int points;
	//! wektor p³ytek na rêce gracza
	std::vector<std::shared_ptr<Tile>> hand;
	//! pozosta³y czas gracza
	sf::Time timeLeft;
	/*!
		G³ówny konstruktor obiektu
		\param id_ identyfikator gracza
		\param timeLeft_ czas na grê
	*/
	Player(int id_,int timeLeft_);
	//! Destruktor
	~Player();
	/*!
		Funkcja rozpoczynaj¹ca turê gracza
		\param bag wskaŸnik na worek z p³ytkami
	*/
	void startTurn(std::shared_ptr<Bag> bag);
	/*!
		Funkcja odpowiadaj¹ca za podniesienie p³ytki
		\param tile p³ytka któr¹ nale¿y podnieœæ
	*/
	void grab(std::shared_ptr<Tile> tile);
	/*!
		Funkcja odpowiadaj¹ca za upuszczenie p³ytki
		\param board wzkaŸnik na planszê
		\param tile p³ytka któr¹ nale¿y upuœciæ
	*/
	void drop(std::shared_ptr<Board> board, std::shared_ptr<Tile> tile);
	/*!
		Funkcja odpowiadaj¹ca za zwrócenie p³ytki z powrotem na rêkê za pomoc¹ prawego przycisku myszki
		\param tile p³ytka któr¹ nale¿y zwróciæ z powrotem na rêkê
	*/
	void backToHand(std::shared_ptr<Tile> tile);
	/*!
		Funkcja odpowiadaj¹ca za zagranie
		\param board wzkaŸnik na planszê
	*/
	void play(std::shared_ptr<Board> board);
	/*!
		Funkcja odpowiadaj¹ca za rozpoczêcie wymiany
		\param board wzkaŸnik na planszê
		\param bag wskaŸnik na worek z p³ytkami
	*/
	void startSwap(std::shared_ptr<Board> board, std::shared_ptr<Bag> bag);
	/*!
		Funkcja odpowiadaj¹ca za zaznaczenie p³ytki do wymiany
		\param tile p³ytka któr¹ nale¿y zaznaczyæ do wymiany
	*/
	void selectForSwap(std::shared_ptr<Tile> tile);
	/*!
		Funkcja odpowiadaj¹ca za zakoñczenie wymiany
		\param bag wskaŸnik na worek z p³ytkami
		\return para: bool - czy nast¹pi³a wymiana, int - ile p³ytek nale¿a³o wymieniæ
	*/
	std::pair<bool,int> endSwap(std::shared_ptr<Bag> bag);
	/*!
		Funkcja odpowiadaj¹ca za spasowanie tury
		\param board wzkaŸnik na planszê
	*/
	void pass(std::shared_ptr<Board> board);
};
