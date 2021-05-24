#pragma warning(push, 0)
#include <SFML/Graphics.hpp>
#pragma warning(pop)
#include "..\Headers\Text.h"

Text::Text(std::string string, sf::Font* font, int size, sf::Vector2f position, bool setOriginToMiddle) {
	text = sf::Text(string, *font, size);
	text.setFillColor(sf::Color::Black);
	text.setPosition(position);
	if(setOriginToMiddle)this->setOriginToMiddle();
}
Text::Text(std::wstring wstring, sf::Font* font, int size, sf::Vector2f position, bool setOriginToMiddle) {
	text = sf::Text(wstring, *font, size);
	text.setFillColor(sf::Color::Black);
	text.setPosition(position);
	if (setOriginToMiddle)this->setOriginToMiddle();
}
Text::Text(){}
Text::~Text(){}

void Text::setString(const std::string& string) {
	text.setString(string);
}
sf::Text* Text::getText() {
	return &text;
}
void Text::setString(const std::wstring& string) {
	text.setString(string);
}
void Text::setPosition(sf::Vector2f position) {
	text.setPosition(position);
}
void Text::setOriginToMiddle() {
	text.setOrigin(text.getLocalBounds().width / 2.0f, text.getLocalBounds().height / 2.0f);
}

