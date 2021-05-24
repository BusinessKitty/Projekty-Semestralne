#pragma once
class Tile;
//! Klasa bêd¹ca reprezentacj¹ worka z p³ytkami
class Bag : public Container{
	//! iloœci odpowiednich liter w worku
	std::map<char, int> quantities = {
		{'A',9},{'a',1},{'B',2},{'C',3},{'c',1},{'D',3},{'E',7},{'e',1},
		{'F',1},{'G',2},{'H',2},{'I',8},{'J',2},{'K',3},{'L',3},{'l',2},
		{'M',3},{'N',5},{'n',1},{'O',6},{'o',1},{'P',3},{'R',4},{'S',4},
		{'s',1},{'T',3},{'U',2},{'W',4},{'Y',4},{'Z',5},{'z',1},{'x',1},{'_',2}
	};
	//! wartoœci punktowe odpowiednich liter
	std::map<char, int> values = {
		{'A',1},{'a',5},{'B',3},{'C',2},{'c',6},{'D',2},{'E',1},{'e',5},
		{'F',5},{'G',3},{'H',3},{'I',1},{'J',3},{'K',2},{'L',2},{'l',3},
		{'M',2},{'N',1},{'n',7},{'O',1},{'o',5},{'P',2},{'R',1},{'S',1},
		{'s',5},{'T',2},{'U',3},{'W',1},{'Y',2},{'Z',1},{'z',5},{'x',9},{'_',0}
	};
public:
	//! Konstruktor bezargumetowy
	Bag();
	//! Destruktor
	~Bag();
	/*!
		Funkcja zwracaj¹ca wskaŸnik na losow¹ p³ytkê z worka
		\return wskaŸnik na wylosowan¹ p³ytkê
	*/
	std::shared_ptr<Tile> grab();
	/*!
		Funkcja "wrzucaj¹ca" p³ytkê do worka
		\param tile wskaŸnik na p³ytkê któr¹ nale¿y wrzuciæ do worka
	*/
	void putBack(std::shared_ptr<Tile> tile);
	/*!
		Funkcja zwracaj¹ca iloœci odpowiednich liter w worku
		\return iloœci odpowiednich liter w worku
	*/
	std::map<char, int> getQuantities();
};