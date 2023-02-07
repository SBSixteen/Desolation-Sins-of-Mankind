#pragma once
#include <SDL.h>
#include <SDL_Mixer.h>
#include "Texture_Handler.h"
#include "Mouse.h"

class Button
{
public:
	SDL_Texture* Texture;
	SDL_Rect sR, dR;
	bool sel, sound, toggle;
	Mix_Chunk* Hover;
	SDL_Renderer* ren;
	Button(SDL_Renderer* R);
	Button(int x, int y, SDL_Renderer* R); //Set Position on Window
	void setSprite(int x, int y); // Pick position from PNG
	void Draw();
	void checkSel(Mouse* mouse); //Check if mouse is on top of it
	void toggleActive();
	~Button();
};

