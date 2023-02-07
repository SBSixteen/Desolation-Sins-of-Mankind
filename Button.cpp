#include "Button.h"
#include <SDL_mixer.h>
#include <iostream>

Button::Button(SDL_Renderer *R) :sR{ 0,0,128,32 }, dR{ 0,0,128,32 } {

	static SDL_Texture* t = Texture_Handler::Load("/Assets/Menu Buttons.png", R);
	Texture = t;
	sel = false;
	ren = R;
	Hover = Mix_LoadWAV("Sounds/Mouse_Hover_Text.wav");
	sound = false;
	toggle = false;
}

Button::Button(int x, int y, SDL_Renderer* R) :sR{ x,y,128,32 }, dR{ x,y,128,32 } {

	//SDL_RenderClear(R);
	//RECT, Texture
	// SDl_RenderCopy(R);
	//SDL_RenderPresent(R);


	static SDL_Texture* t = Texture_Handler::Load("Assets/Menu Buttons.png", R);
	Texture = t;
	sel = false;
	ren = R;
	Hover = Mix_LoadWAV("Sounds/Mouse_Hover_Text.wav");
	sound = false;
	toggle = false;
}

void Button::toggleActive() {

	if(!toggle) {

		sR.x = 256;
		toggle = true;
	}else {
		toggle = false;
		sR.x = 0;
	}

}

Button::~Button() {

}

void Button::checkSel(Mouse* mouse) {

	if (SDL_HasIntersection(&(mouse->tip), &dR)) {

		sel = true;

		sR.x = 128;

		if (!sound) {
			Mix_PlayChannel(-1, Hover, 0);
			sound = true;
		}
	}
	else {
		if (!toggle) {
			sR.x = 0;
		}
		else {
			sR.x = 256;
		}
		sel = false;
		sound = false;
	}
}

void Button::setSprite(int x, int y) {

	dR.x = x;
	dR.y = y;

}

void Button::Draw() {
	SDL_RenderCopy(ren, Texture, &sR, &dR);
}