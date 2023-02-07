#include "Enemy.h"
#include <iostream>
#include <SDL_image.h>

Enemy::Enemy(int x, int y, int h, int s, int d, SDL_Renderer* R) {

	int random = rand() % 100;

	Ren = R;
	target = false;

	BB = new SDL_Rect();
	BB->x = x; BB->y = y; BB->w = 128; BB->h = 128;
	//HB = new SDL_Rect();
	//HB->x = 64; HB->y = 64; HB->w = 30; HB->h = 30;

	alive = true;

	if (random < 80) {

		//Normal Shadow
		// VERY WEAK, NORMAL SPEED, BAD DAMAGE

		health = 1 + h;
		speed = 2 + s;
		damage = 20 + d * 3;

		random = rand() % 3;

		if (random == 0) {
			Art = IMG_LoadTexture(R, "Assets/Enemies/Shadow_1.png");
		}
		if (random == 1) {
			Art = IMG_LoadTexture(R, "Assets/Enemies/Shadow_2.png");
		}
		if (random == 2) {
			Art = IMG_LoadTexture(R, "Assets/Enemies/Shadow_3.png");
		}
		return;
	}

	if (random < 85) {

		//PHAT
		// TANK, SLOW SPEED, GOOD DAMAGE

		health = 10 + (2 * h);
		speed = 2 + s;
		damage = 43 + d * 3;

		Art = IMG_LoadTexture(R, "Assets/Enemies/Shadow_PHAT.png");
		return;
	}

	if (random < 90) {

		//STRONG
		//SRONG, SLOW SPEED, GREAT DAMAGE

		health = 4 + 2 * h;
		speed = 2 + s;
		damage = 56 + d * 10;

		Art = IMG_LoadTexture(R, "Assets/Enemies/Shadow_Strong.png");
		return;
	}

	if (random < 95) {

		//BERSERKER
		// WEAK, SUPER SPEED, NORMAL DAMAGE

		health = 2 + 1 * h;
		speed = 3 + s;
		damage = 39 + d * 5;

		Art = IMG_LoadTexture(R, "Assets/Enemies/Shadow_Berserker.png");
		return;
	}

	if (random < 100) {

		//ABOMINATION
		// SUPER STRONG, NORMAL SPEED, EXCELLENT DAMAGE

		health = 7 + 1 * h;
		speed = 2 + s;
		damage = 66 + d * 5;

		Art = IMG_LoadTexture(R, "Assets/Enemies/Shadow_Abomination.png");
		return;
	}

};

void Enemy::move(int W, int H) {

	//Move to center of viewport

	float a = W - BB->x -64;
	float b = BB->y - H + 64;

	rads = atan2f(a,b);
	deg = ((rads * 360) / (2 * 3.142)) - 90;

	int dy = 1 * (speed * cos(rads));
	int dx = 1 * (speed * sin(rads));


	BB->x = BB->x + dx;
	BB->y = BB->y - dy;

};

void Enemy::in_aim(Mouse* M) {

	if (SDL_HasIntersection(&(M->tip), BB)) {

		target = true;

	}
	else {
		target = false;
	}
};

void Enemy::hurt() {

	health--;
	if (health <= 0) {
		alive = false;
	}

};

void Enemy::relocate(int x, int y) {

	BB->x = BB->x + x;
	BB->y = BB->y + y;

};

void Enemy::draw() {
	SDL_RenderCopyEx(Ren, Art, NULL, BB, deg, NULL, SDL_FLIP_NONE );
	//SDL_RenderCopy(Ren, Art, NULL, BB);
}

