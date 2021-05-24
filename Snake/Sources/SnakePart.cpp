#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
using namespace std;
using namespace sf;

#include "..\Headers\GameInfo.h"
#include "..\Headers\Tile.h"
#include "..\Headers\SnakePart.h"
#include "..\Headers\Fruit.h"
#include "..\Headers\Global.h"

SnakePart::SnakePart(){}
SnakePart::SnakePart(int x_, int y_,int width_,double scale_, const char* filename_, SnakePart* next_, SnakePart* prev_) :Tile(x_,y_,width_,scale_,filename_),next(next_),prev(prev_){}
SnakePart::~SnakePart(){}

void SnakePart::updateSnake(int width){
	auto w = this;
	while (w) {
		w->updatePosition(width);
		w = w->next;
	}
}
void SnakePart::draw(SnakePart*& head, RenderWindow* window,double scale) {

	auto w = head->next;
	while (w) {
		if(!w->next){
			//w - ogon
			if (w->getX() < w->prev->getX()) w->loadSprite(scale, "Resources/tail_r.png");
			else if (w->getX() > w->prev->getX()) w->loadSprite(scale, "Resources/tail_l.png");
			else if (w->getY() < w->prev->getY()) w->loadSprite(scale, "Resources/tail_d.png");
			else if (w->getY() > w->prev->getY()) w->loadSprite(scale, "Resources/tail_u.png");
			else{
				if(!w->prev->prev) w->loadSprite(scale, "Resources/tail_u.png");
				else {
					if (w->prev->getX() < w->prev->prev->getX()) w->prev->loadSprite(scale, "Resources/tail_r.png");
					else if (w->prev->getX() > w->prev->prev->getX()) w->prev->loadSprite(scale, "Resources/tail_l.png");
					else if (w->prev->getY() < w->prev->prev->getY()) w->prev->loadSprite(scale, "Resources/tail_d.png");
					else if (w->prev->getY() > w->prev->prev->getY()) w->prev->loadSprite(scale, "Resources/tail_u.png");
				}
			}
		}
		else {
			if ((w->next->getX() < w->getX() && w->prev->getX() > w->getX()) || (w->next->getX() > w->getX() && w->prev->getX() < w->getX())) w->loadSprite(scale, "Resources/body_horizontal.png");
			else if ((w->next->getY() < w->getY() && w->prev->getY() > w->getY()) || (w->next->getY() > w->getY() && w->prev->getY() < w->getY())) w->loadSprite(scale, "Resources/body_vertical.png");
			else if ((w->next->getX() < w->getX() && w->prev->getY() > w->getY()) || (w->next->getY() > w->getY() && w->prev->getX() < w->getX())) w->loadSprite(scale, "Resources/body_dl.png");
			else if ((w->next->getY() > w->getY() && w->prev->getX() > w->getX()) || (w->next->getX() > w->getX() && w->prev->getY() > w->getY())) w->loadSprite(scale, "Resources/body_dr.png");
			else if ((w->next->getX() > w->getX() && w->prev->getY() < w->getY()) || (w->next->getY() < w->getY() && w->prev->getX() > w->getX())) w->loadSprite(scale, "Resources/body_ur.png");
			else if ((w->next->getX() < w->getX() && w->prev->getY() < w->getY()) || (w->next->getY() < w->getY() && w->prev->getX() < w->getX())) w->loadSprite(scale, "Resources/body_ul.png");
			else {}
		}
		window->draw(*(w->getSprite()));
		w = w->next;
	}

	if (head->next->getX() > head->getX()) head->loadSprite(scale, "Resources/head_l.png");
	else if (head->next->getX() < head->getX()) head->loadSprite(scale, "Resources/head_r.png");
	else if (head->next->getY() > head->getY()) head->loadSprite(scale, "Resources/head_u.png");
	else if (head->next->getY() < head->getY()) head->loadSprite(scale, "Resources/head_d.png");
	else head->loadSprite(scale, "Resources/head_u.png");
	window->draw(*(head->getSprite()));
}
void SnakePart::move(SnakePart*& head) {
	if (head->next->next) move(head->next);
	head->next->setX(head->getX());
	head->next->setY(head->getY());
}
void SnakePart::grow(SnakePart*& head, GameInfo* game) {
	auto w = head;
	while (w->next) w = w->next;
	w->next = new SnakePart(w->getX(), w->getY(), game->width, game->scale, "Resources/transparent.png", nullptr,w);
}
void SnakePart::checkCollision(SnakePart*& head, GameInfo* game){
	if ((head->getX() < 0 && game->direction == 2) || (head->getX() > (game->size) - 1 && game->direction == 3) || (head->getY() < 0 && game->direction == 0) || (head->getY() > (game->size) - 1) && game->direction == 1) {
		game->state = 3;
	}
	//cout << head->getX() << " " << head->getY() << endl;
	auto w = head->next;
	while (w) {
		if (*w==*head) {
			//cout << w->getX() << " " << w->getY() << endl;
			game->state = 3;
			break;
		}
		w = w->next;
	}
}