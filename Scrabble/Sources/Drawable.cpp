#pragma warning(push, 0)
#include <SFML/Graphics.hpp>
#pragma warning(pop)


#include "..\Headers\Drawable.h"

Drawable::Drawable(sf::Vector2f coords_, std::string filename)
	:coords(coords_) 
{
	setSprite(filename);
}
Drawable::Drawable() {};
Drawable::~Drawable() {};

void Drawable::setSprite(const std::string& filename) {
	texture.loadFromFile(filename);
	sprite.setTexture(texture);
	sprite.setPosition(coords);
	bounds = sprite.getGlobalBounds();
}
sf::Sprite* Drawable::getSprite() { return &sprite; }

sf::FloatRect* Drawable::getBounds() { return &bounds; }

void Drawable::setCoords(sf::Vector2f coords_) {
	coords = coords_;
	getSprite()->setPosition(coords);
	bounds = sprite.getGlobalBounds();
}
sf::Vector2f Drawable::getCoords() { return coords; }