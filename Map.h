#pragma once
#include <SDL.h>
class Map
{
	Map();
	~Map();
	
	void Load(int arr, int x, int y);
	void Draw();

private:
	SDL_Rect sR, dR;
	SDL_Texture* T_1;
	SDL_Texture* T_2;
	SDL_Texture* T_3;
	SDL_Texture* T_4;
	SDL_Texture* T_5;

	int map[20][20];
};

