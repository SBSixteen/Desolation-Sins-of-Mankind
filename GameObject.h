#pragma once
#include <SDL.h>
class GameObject
{
public:
	GameObject(const char* T_Sheet, SDL_Renderer* R, int x, int y);
	~GameObject();

	void Update();
	void Render();

private:
	int xpos, ypos;



	SDL_Texture* Texture;
	SDL_Rect sR, dR;

	SDL_Renderer* ren;
};

