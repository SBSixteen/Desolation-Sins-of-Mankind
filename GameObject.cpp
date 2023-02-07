#include "GameObject.h"
#include "Texture_Handler.h" 

GameObject::GameObject(const char* path, SDL_Renderer* R, int x, int y) {

	ren = R;
	Texture = Texture_Handler::Load(path, R);

	xpos = x;
	ypos = y;
}

void GameObject::Update() {

	xpos++;

	sR.h = 32; sR.w = 32; sR.x = 0; sR.y = 0;
	dR.x = xpos; dR.y = ypos; dR.w = sR.w * 2; dR.h = sR.h * 2;
}

void GameObject::Render() {
	SDL_RenderCopy(ren, Texture, &sR, &dR);
}