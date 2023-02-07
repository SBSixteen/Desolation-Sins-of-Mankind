#pragma once

#include <SDL.h>
#include <stdio.h>

class GameDev_Project {

public:
	static void execute(bool* isRunning);
	static void update(int* c);
	static void render(SDL_Renderer* R);
	void flush();

private:

};

static void execute();
