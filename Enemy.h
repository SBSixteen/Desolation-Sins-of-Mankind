#pragma once
#include <SDL.h>
#include "Mouse.h"
class Enemy
{
public:
	int health; int speed; int damage;
	float rads; float deg;
	bool alive, target;
	SDL_Rect* BB; //SDL_Rect* HB;
	SDL_Texture* Art;
	SDL_Renderer* Ren;

	Enemy(int x, int y, int h_modif, int s_modif, int d_modif, SDL_Renderer* R) ;
	void move(int W, int H);
	void relocate(int x, int y);
	void in_aim(Mouse* M);
	void hurt();
	void draw();
};

