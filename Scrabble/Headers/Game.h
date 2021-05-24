#pragma once
class Player;
class WindowManager;
//! Klasa zarz¹dzaj¹ca przebiegiem gry
class Game {
public:
	//! enum okreœlaj¹cy mo¿liwe stany gry
	enum class State{
		IDLE = 0,		/*!< stan domyœlny */
		PICKED,			/*!< aktualny gracz trzyma p³ytkê */
		SWAPPING,		/*!< aktualny gracz wymienia litery */
		CHECKING_TILES, /*!< aktualny gracz sprawdza litery pozosta³e w worku */
		SURRENDERED,	/*!< aktualny gracz podda³ grê */
		TIME_OUT		/*!< czas aktualnego gracza skoñczy³ siê */
	};
	/*!
		G³ówny konstruktor obiektu
		\param rules_ zasady gry
	*/
	Game(std::string rules_, int time_);
	//! Destruktor
	~Game();
	/*!
		Funkcja odpowiadaj¹ca za przebieg gry
		\return informacja o tym, czy zacz¹æ now¹ grê
	*/
	int play();
private:
	//! g³ówne okno gry
	sf::RenderWindow window;
	//! czcionka u¿ywana w programie
	sf::Font font;
	//! ikona gry
	sf::Image icon;
	//! t³o okna
	Drawable background;
	//! t³o planszy
	sf::RectangleShape boardBackground;
	//! zasady gry
	std::string rules;
	//! czas graczy w sekundach
	int time;
	//! aktualny stan gry
	State state;
	//! zmienna pomocnicza do zapisywania ile spasowañ nast¹pi³o z rzêdu
	int passCount;
	//! mapa zawieraj¹ca obiekty typu drawable przedstawiaj¹ce przyciski
	std::map<std::string, std::unique_ptr<Drawable>> buttons;
	//! mapa zawieraj¹ca obiekty typu text przedstawiaj¹ce teksty
	std::map<std::string, Text> texts;
	//! wskaŸnik na gracza aktualnie odbywaj¹cego turê
	std::shared_ptr<Player> currentPlayer;
	//! wskaŸnik na gracza czekaj¹cego na turê
	std::shared_ptr<Player> nextPlayer;
	//! wskaŸnik na planszê
	std::shared_ptr<Board> board;
	//! wskaŸnik na worek z p³ytkami
	std::shared_ptr<Bag> bag;
	//! zmienna pomocnicza zawieraj¹ca wektor aktualnie tworzonych s³ów oraz sumaryczn¹ iloœæ punktów mo¿liw¹ do zdobycia za nie
	std::pair<std::vector<std::string>, int> currPlayerCheck;
	/*!
		Funkcja sprawdzaj¹ca czy zosta³y spe³nione warunki koñca gry
		\return 0 - koniec gry, nie rozpoczynaæ nastêpnej, 1 - koniec gry, rozpocz¹æ nastêpn¹, 2 - gra trwa nadal
	*/
	int checkEndConditions();
	/*!
		Funkcja odpowiadaj¹ca za obs³ugê zdarzeñ
		\param c wskaŸnik na obiekt typu sf::Clock
	*/
	void handleEvents(sf::Clock* c);
	/*!
		Funkcja wykonuj¹ca operacje, które nale¿y wykonaæ co turê
	*/
	void nextTurn();
	/*!
		Funkcja zamieniaj¹ca obiekt typu sf::Time na ci¹g znaków zawieraj¹cy czas w formacie "MM:SS"
		\param time wskaŸnik na obiekt sf:Time który nale¿y zamieniæ na ci¹g znaków
		\return po¿¹dany ci¹g znaków
	*/
	std::string sfTimeToString(sf::Time* time);
	/*!
		Funkcja inicjalizuj¹ca przyciski
	*/
	void initializeButtons();
	/*!
		Funkcja inicjalizuj¹ca teksty
	*/
	void initializeTexts();
};