#pragma warning(push, 0)
#include <SFML/Graphics.hpp>
#pragma warning(pop)
#include <memory>

#include "..\Headers\Text.h"
#include "..\Headers\Drawable.h"
#include "..\Headers\Container.h"

Container::Container() {}
Container::~Container() {}

int Container::size() {
	return contents.size();
}
std::vector<std::shared_ptr<Drawable>>::iterator Container::begin() {
	return contents.begin();
}
std::vector<std::shared_ptr<Drawable>>::iterator Container::end() {
	return contents.end();
}