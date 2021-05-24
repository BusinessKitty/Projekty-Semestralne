#pragma once
class Player;
class WindowManager;
//! Klasa zarz�dzaj�ca przebiegiem gry
class Game {
public:
	//! enum okre�laj�cy mo�liwe stany gry
	enum class State{
		IDLE = 0,		/*!< stan domy�lny */
		PICKED,			/*!< aktualny gracz trzyma p�ytk� */
		SWAPPING,		/*!< aktualny gracz wymienia litery */
		CHECKING_TILES, /*!< aktualny gracz sprawdza litery pozosta�e w worku */
		SURRENDERED,	/*!< aktualny gracz podda� gr� */
		TIME_OUT		/*!< czas aktualnego gracza sko�czy� si� */
	};
	/*!
		G��wny konstruktor obiektu
		\param rules_ zasady gry
	*/
	Game(std::string rules_, int time_);
	//! Destruktor
	~Game();
	/*!
		Funkcja odpowiadaj�ca za przebieg gry
		\return informacja o tym, czy zacz�� now� gr�
	*/
	int play();
private:
	//! g��wne okno gry
	sf::RenderWindow window;
	//! czcionka u�ywana w programie
	sf::Font font;
	//! ikona gry
	sf::Image icon;
	//! t�o okna
	Drawable background;
	//! t�o planszy
	sf::RectangleShape boardBackground;
	//! zasady gry
	std::string rules;
	//! czas graczy w sekundach
	int time;
	//! aktualny stan gry
	State state;
	//! zmienna pomocnicza do zapisywania ile spasowa� nast�pi�o z rz�du
	int passCount;
	//! mapa zawieraj�ca obiekty typu drawable przedstawiaj�ce przyciski
	std::map<std::string, std::unique_ptr<Drawable>> buttons;
	//! mapa zawieraj�ca obiekty typu text przedstawiaj�ce teksty
	std::map<std::string, Text> texts;
	//! wska�nik na gracza aktualnie odbywaj�cego tur�
	std::shared_ptr<Player> currentPlayer;
	//! wska�nik na gracza czekaj�cego na tur�
	std::shared_ptr<Player> nextPlayer;
	//! wska�nik na plansz�
	std::shared_ptr<Board> board;
	//! wska�nik na worek z p�ytkami
	std::shared_ptr<Bag> bag;
	//! zmienna pomocnicza zawieraj�ca wektor aktualnie tworzonych s��w oraz sumaryczn� ilo�� punkt�w mo�liw� do zdobycia za nie
	std::pair<std::vector<std::string>, int> currPlayerCheck;
	/*!
		Funkcja sprawdzaj�ca czy zosta�y spe�nione warunki ko�ca gry
		\return 0 - koniec gry, nie rozpoczyna� nast�pnej, 1 - koniec gry, rozpocz�� nast�pn�, 2 - gra trwa nadal
	*/
	int checkEndConditions();
	/*!
		Funkcja odpowiadaj�ca za obs�ug� zdarze�
		\param c wska�nik na obiekt typu sf::Clock
	*/
	void handleEvents(sf::Clock* c);
	/*!
		Funkcja wykonuj�ca operacje, kt�re nale�y wykona� co tur�
	*/
	void nextTurn();
	/*!
		Funkcja zamieniaj�ca obiekt typu sf::Time na ci�g znak�w zawieraj�cy czas w formacie "MM:SS"
		\param time wska�nik na obiekt sf:Time kt�ry nale�y zamieni� na ci�g znak�w
		\return po��dany ci�g znak�w
	*/
	std::string sfTimeToString(sf::Time* time);
	/*!
		Funkcja inicjalizuj�ca przyciski
	*/
	void initializeButtons();
	/*!
		Funkcja inicjalizuj�ca teksty
	*/
	void initializeTexts();
};