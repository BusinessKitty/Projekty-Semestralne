#pragma once
class Game;
class Player;

//! Klasa odpowiadająca za zarządznie oknami sf::RenderWindow
class WindowManager : public Singleton<WindowManager> {
private:
	//! tło okien
	static Drawable background;
	//! czcionka używana w programie
	static sf::Font font;
	//! ikona gry
	static sf::Image icon;
public:
	//! Główny konstruktor
	WindowManager(token);
	//! Destruktor
	~WindowManager();
	/*!
		Funkcja zarządzająca ekranem ładowania
	*/
	void loadingScreen();
	/*!
		Funkcja zarządzająca ekranem wyboru zasad gry
		\return string "pass", "empty" lub "both" określający zasady gry oraz int zawierający czas pojedynczego gracza w minutach
	*/
	std::pair<std::string,int> chooseConditionScreen();
	/*!
		Funkcja zarządzająca ekranem pokazującym pozostałe płytki w worku
		\param bag wskaźnik na worek
	*/
	static void tilesLeftScreen(std::shared_ptr<Bag> bag);
	/*!
		Funkcja zarządzająca ekranem potwierdzenia wyjścia z programu
		\return true, jeżeli użytkownik chce wyjść z programu
	*/
	static bool escapePromptScreen();
	/*!
		Funkcja zarządzająca ekranem potwierdzenia poddania się
		\return true, jeżeli użytkownik chce się poddać
	*/
	static bool surrenderPromptScreen();
	/*!
		Funkcja zarządzająca ekranem końca gry
		\param state stan, którym zakończyła się gra
		\param currentPlayer wskaźnik na aktualnego gracza
		\param nextPlayer wskaźnik na następnego gracza
		\return true, jeżeli użytkownik chce rozpocząć nową grę
	*/
	static bool endScreen(const Game::State& state, std::shared_ptr<Player> currentPlayer, std::shared_ptr<Player> nextPlayer);
	/*!
		Funkcja zarządzająca ekranem wyboru litery na miejsce pustej płytki
		\param bag wskaźnik na worek
		\param blank wskaźnik na pustą płytkę
	*/
	static void pickLetterScreen(std::shared_ptr<Bag> bag, std::shared_ptr<Blank> blank);
};