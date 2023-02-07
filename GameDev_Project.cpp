
#define SDL_MAIN_HANDLED
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <iostream>
#include <fstream>
#include "GameDev_Project.h"
#include "Texture_Handler.h"
#include <SDL.h>
#include <SDL_image.h>
#include "SDL_ttf.h"
#include <SDL_mixer.h>
#include "wtypes.h"
#include "GameObject.h"
#include "Button.h"
#include "Mouse.h"
#include <experimental/filesystem>
#include <string>
#include "LinkedList.h"
using namespace std;

class Profile
{
public:
	string name = "";
	int rank, kills, deaths, shots_taken, money, dd, mm, yy, cdd, cmm, cyy;
	float kd, ke;
	double xp, seconds_played;
	bool weapons[12];

	void create(string N) {
		struct tm newtime;
		time_t now = time(0);
		localtime_s(&newtime, &now);

		name = N;
		rank = 0;
		kills = 0;
		deaths = 0;
		shots_taken = 0;
		money = 0;
		cyy = 1900 + newtime.tm_year;
		cmm = 1 + newtime.tm_mon;
		cdd = newtime.tm_mday;

		kd = 0.0f;
		ke = 0.0f;

		xp = 0;
		seconds_played = 0;
	}

};

namespace fs = std::experimental::filesystem;

bool isRunning = false;
int c = 0;
SDL_Renderer* R;
GameObject* player;
Mouse* MenuMouse;
Button* button[7];
Button* profile_button[2];
enum ButtonNames { PLAY, EXIT, PROFILES, LOADOUT, STATS, SHOP, FS };

static void write(Profile A) {

	string format = ".prof";
	string fn = "C:\\Game User Files\\" + A.name + format;

	ofstream file1{ fn };
	//file1.open(fn, ios::app);
	file1.write((char*)&A, sizeof(A));
	file1.close();

}

static void update_last_seen(Profile A) {

	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);

	A.yy = 1900 + newtime.tm_year;
	A.mm = 1 + newtime.tm_mon;
	A.cdd = newtime.tm_mday;

}

static Profile read(string filename) {

	Profile A;

	ifstream file2;
	//file2.open(filename, ios::in);
	file2.seekg(0);
	file2.read((char*)&A, sizeof(A));

	return A;
}

static void execute() {

	SDL_Event e;
	SDL_PollEvent(&e);

	switch (e.type) {
	case SDL_QUIT:
		isRunning = false;
		break;

	default:
		break;
	}

}

static void render() {

	SDL_RenderClear(R);

	//player->Render();

	SDL_RenderPresent(R);

}

static void update() {

	player->Update();

}

void menuUpdate() {

	for (Button* n : button) {

		n->checkSel(MenuMouse);
		n->Draw();

	}

	MenuMouse->Draw();
}

void profileUpdate() {

	for (Button* n : profile_button) {

		n->checkSel(MenuMouse);
		n->Draw();

	}

	MenuMouse->Draw();
}



int main()
{
	//First Time Setup maybe
	TTF_Init(); // Font Library Initializer
	Profile LOADED;

	int frames = 0; //Ticks
	if (!fs::is_directory("C:\\Game User Files") || !fs::exists("C:\\Game User Files")) {
		fs::create_directory("C:\\Game User Files");
	}

	//Fullscreen
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	const int WIDTH = desktop.right, HEIGHT = desktop.bottom, //Desktop Dimensions
		//const int WIDTH = 500, HEIGHT = 500,
		FPS = 144, delay = 1000 / FPS; //Pure Frametime, 
	//1 fps = 1tick
	Uint32 frame_start;
	int frame_time;

	SDL_Surface* WindowSurface;


	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {

		std::cout << "SDL Failed to initialize. Error Code : " << SDL_GetError() << std::endl;

	}

	SDL_Window* window = SDL_CreateWindow("DESOLATION : Sins of our Forefathers | Version 1.0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
	WindowSurface = SDL_GetWindowSurface(window); //Creates Window Panel

	if (window == NULL) {
		std::cout << "Could not initialize SDL Window. Error Code : " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 8192) < 0) //2 = Stereo
	{
		std::cout << "SDL_mixer could not initialize! SDL_mixer Error: %s\n" << Mix_GetError() << std::endl;
		//success = false;
	} //Sound Mixer Initializer

	Mix_Music* Menu_Music = Mix_LoadMUS("Sounds/Menu_Music.mp3");

	R = SDL_CreateRenderer(window, -1, 0); //Renders Textures

	if (R) {
		std::cout << "Renderer Initialized" << std::endl;
	}

	SDL_SetWindowFullscreen(window, false);
	bool fullscreen = false;

	isRunning = true;

	SDL_Texture* DUMP;

	player = new GameObject("Assets/Red.png", R, 0, 0);

	//Play = 0

	button[PLAY] = new Button(0, 0, R);
	button[PLAY]->setSprite(WIDTH / 2 - 64, HEIGHT / 2 - 16 + 100);
	button[EXIT] = new Button(0, 32, R);
	button[EXIT]->setSprite(WIDTH / 2 - 64, HEIGHT / 2 - 16 + 64 * 3 + 100);
	button[PROFILES] = new Button(0, 64, R);
	button[PROFILES]->setSprite(0 + 32, HEIGHT - 80);
	button[LOADOUT] = new Button(0, 96, R);
	button[LOADOUT]->setSprite(WIDTH / 2 - 64, HEIGHT / 2 + -16 + (64 * 2) + 100);
	button[STATS] = new Button(0, 128, R);
	button[STATS]->setSprite(WIDTH - 160, HEIGHT - 80);
	button[SHOP] = new Button(0, 160, R);
	button[SHOP]->setSprite(WIDTH / 2 - 64, HEIGHT / 2 + -16 + (64 * 1) + 100);
	button[FS] = new Button(0, 192, R);
	button[FS]->setSprite(WIDTH - 96, 16);

	//Create
	profile_button[0] = new Button(0, 224, R);
	profile_button[0]->setSprite(WIDTH - 128, HEIGHT - 64);

	//Delete
	profile_button[1] = new Button(0, 32, R);
	profile_button[1]->setSprite(0, HEIGHT - 64);

	bool menu = false; //Inside Menu Screen
	bool play = false; //Inside Gameplay Screen
	bool profile = true; //Inside Profile Screen

	int psy = 12;
	int psx = 12; //Player Speed

	SDL_Texture* BG = IMG_LoadTexture(R, "Assets/BG.jpg");
	SDL_Texture* Splash = IMG_LoadTexture(R, "Assets/Game Splash.png");
	SDL_Texture* F_AID = IMG_LoadTexture(R, "Assets/Health_Icon.png");

	srand(time(NULL)); //Time Seed = 0

	//int R = rand();

	SDL_Rect* Level_KS = new SDL_Rect();
	Level_KS->x = 0; Level_KS->y = 0; Level_KS->w = 2200, Level_KS->h = 2200;
	SDL_Texture* Kikowani_Station = IMG_LoadTexture(R, "Maps/Map_Kikowani_Station.png");

	SDL_Texture* info_reload = IMG_LoadTexture(R, "Assets/Reload_Warning.png");
	SDL_Rect* show_info_reload = new SDL_Rect();

	SDL_QueryTexture(info_reload, NULL, NULL, &show_info_reload->w, &show_info_reload->h); //Takes the W and H of the texture, puts into SDL_Rect();

	//Anims
	SDL_Texture* player_still[20]; //Idling Animations
	SDL_Texture* player_reload[15]; //Reload Animation
	SDL_Texture* Mag_Anim[14]; //Magazine Simulation

	string idle_anim_path = "Assets/Player/survivor-idle_handgun_";

	//IDLE ANIMS
	for (int i = 0; i < 20; i++) {
		idle_anim_path.append(to_string(i));
		idle_anim_path.append(".png");
		player_still[i] = IMG_LoadTexture(R, idle_anim_path.c_str());
		idle_anim_path = "Assets/Player/survivor-idle_handgun_";

	}

	idle_anim_path = "Assets/Reload/survivor-reload_handgun_";

	//RELOAD
	for (int i = 0; i < 15; i++) {
		idle_anim_path.append(to_string(i));
		idle_anim_path.append(".png");
		//printf(idle_anim_path.c_str());
		player_reload[i] = IMG_LoadTexture(R, idle_anim_path.c_str());
		idle_anim_path = "Assets/Reload/survivor-reload_handgun_";
	}

	idle_anim_path = "Assets/Mag_Anim/Magazine_P99_";

	//Mag_Size
	for (int i = 0; i < 14; i++) {
		idle_anim_path.append(to_string(i));
		idle_anim_path.append(".png");
		//printf(idle_anim_path.c_str());
		Mag_Anim[i] = IMG_LoadTexture(R, idle_anim_path.c_str());
		idle_anim_path = "Assets/Mag_Anim/Magazine_P99_";
	}

	SDL_Rect* splash = new SDL_Rect();
	splash->x = 0; splash->y = 100; splash->w = 1920, splash->h = 400;

	SDL_Rect* s_splash = new SDL_Rect();
	s_splash->x = WIDTH / 2 - 480; s_splash->y = 100; s_splash->w = 960, s_splash->h = 200;


	//ALL the SOUND EFFECTS
	Mix_Chunk* Door_Creak = Mix_LoadWAV("Sounds/Door_Creak.wav");
	Mix_Chunk* Door_Slam = Mix_LoadWAV("Sounds/Door_Slam.wav");
	Mix_Chunk* Gunshot = Mix_LoadWAV("Sounds/Gunshot_p99.wav");
	Mix_Chunk* Reload = Mix_LoadWAV("Sounds/Reload.wav");
	Mix_Chunk* Empty = Mix_LoadWAV("Sounds/Gun_Empty.wav");
	Mix_Chunk* Player_Hurt = Mix_LoadWAV("Sounds/Player_Grunt.wav");
	Mix_Chunk* Ammo_Restock = Mix_LoadWAV("Sounds/Ammo_Refill.wav");
	Mix_Chunk* Devil_Laugh = Mix_LoadWAV("Sounds/Devil_Laugh.wav");

	Mix_Chunk* Spawn_1 = Mix_LoadWAV("Sounds/Spawn_1.wav");
	Mix_Chunk* Spawn_2 = Mix_LoadWAV("Sounds/Spawn_2.wav");
	Mix_Chunk* Spawn_3 = Mix_LoadWAV("Sounds/Spawn_3.wav");
	Mix_Chunk* Spawn_4 = Mix_LoadWAV("Sounds/Spawn_4.wav");


	//MUSIC
	Mix_Music* Gameplay = Mix_LoadMUS("Sounds/Kikowani_Station.mp3");
	Mix_Music* End = Mix_LoadMUS("Sounds/Game_Over.mp3");;

	//Crosshair
	MenuMouse = new Mouse(R);
	Mouse* Crosshair = new Mouse(R, 1);

	//Fonts Loading
	TTF_Font* username = TTF_OpenFont("Other/DEADLY KILLERS.ttf", 128);
	TTF_Font* other = TTF_OpenFont("Other/1 DIMENCION.ttf", 72);
	TTF_Font* Record = TTF_OpenFont("Other/Record.ttf", 128);
	TTF_Font* EXO = TTF_OpenFont("Other/Exo2.ttf", 32);

	//Remove Cursor
	SDL_ShowCursor(SDL_DISABLE);

	SDL_Event e;

	LinkedList Shadows; //Holds all the enemies

	bool invulnerable = false; //true -> 2secs later -> false;
	int I = 0;

	//

	while (profile) {

		frame_start = SDL_GetTicks(); //Starting Time

		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:

			{
				profile = false;
				break;
			}

			case SDL_MOUSEBUTTONUP:

				if (e.button.button == SDL_BUTTON_LEFT) {
					if (profile_button[1]->sel) {
						profile = false;
					}
					if (profile_button[0]->sel) {

						bool quit = false;
						SDL_RenderClear(R);

						SDL_Color textColor = { 0, 0, 0, 0xFF };

						string inputText = "Your First Name";

						SDL_Rect* name = new SDL_Rect();

						SDL_Surface* ACTIVITY = TTF_RenderText_Solid(username, inputText.c_str(), { 255,255,255 }); //Surface = Canvas
						SDL_Texture* FF = SDL_CreateTextureFromSurface(R, ACTIVITY);

						SDL_QueryTexture(FF, NULL, NULL, &name->w, &name->h);

						name->x = WIDTH / 2 - name->w / 2; name->y = HEIGHT / 2 - name->h / 2;

						SDL_StartTextInput();

						while (!quit)
						{
							//The rerender text flag
							bool renderText = false;

							//Handle events on queue
							while (SDL_PollEvent(&e) != 0)
							{

								if (e.type == SDL_TEXTINPUT)
								{
									//Accept Keyboard Input
									if (!(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')))
									{
										//Append character
										inputText += e.text.text;
										renderText = true;
									}
								}
								else if (e.type == SDL_KEYDOWN)
								{
									//Handle backspace
									if (e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0)
									{
										//lop off character
										inputText.pop_back();
										renderText = true;
									}
									//Submit Name
									if (e.key.keysym.sym == SDLK_RETURN && inputText.length() > 0)
									{
										quit = true;
										menu = true;
										profile = false;
										Profile NEW; NEW.create(inputText); //WEE ARE NOT USING PROFILES ANYMORE
										write(NEW);
										update_last_seen(NEW);

										LOADED = NEW;
										Mix_PlayChannel(-1, Door_Creak, 0); //Stereo
										Mix_PlayMusic(Menu_Music, -1);


									}
									if (e.key.keysym.sym == SDLK_ESCAPE)
									{
										quit = true;
										profile = true;

										Mix_PlayChannel(-1, Door_Creak, 0);

									}
								}

							}

							SDL_RenderClear(R);

							SDL_RenderCopy(R, FF, NULL, name);
							SDL_RenderPresent(R);

							if (renderText) {
								ACTIVITY = TTF_RenderText_Solid(username, inputText.c_str(), { 255,255,255 });
								DUMP = FF;
								FF = SDL_CreateTextureFromSurface(R, ACTIVITY);
								SDL_FreeSurface(ACTIVITY);
								SDL_QueryTexture(FF, NULL, NULL, &name->w, &name->h);
								name->x = WIDTH / 2 - name->w / 2; name->y = HEIGHT / 2 - name->h / 2;

								SDL_RenderClear(R);
								SDL_RenderCopy(R, FF, NULL, name);
								SDL_RenderPresent(R);
								SDL_DestroyTexture(DUMP);

							}


						}

						SDL_DestroyTexture(FF);

					}
				}


			}
		}


		SDL_RenderClear(R);
		SDL_GetMouseState(&(MenuMouse->cursor.x), &(MenuMouse->cursor.y));
		SDL_RenderCopy(R, Splash, NULL, s_splash);
		profileUpdate();
		SDL_RenderPresent(R);

		//menu = true;

		while (menu) {


			while (SDL_PollEvent(&e)) {
				switch (e.type) {
				case SDL_QUIT:
					break;
				case SDL_MOUSEBUTTONUP:
					if (e.button.button == SDL_BUTTON_LEFT) {
						if (button[EXIT]->sel) {
							menu = false;
							profile = false;
						}
						if (button[PROFILES]->sel) {
							menu = false;
							profile = true;
						}
						if (button[PLAY]->sel) {

							play = true;

							Mix_PauseMusic();

							//We will discuss this in the future
							for (int i = 0; i < Shadows.length; i++)
							{
								Shadows.deleteIndex(i);
							}

							SDL_RenderClear(R);

							const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL); //Array that holds the key states of the keyboard

							//currentKeyStates[SDL_K];

							int X = 0; int Y = 0; //Frame Controllers, X = IDLE, Y = Reload

							SDL_Surface* Map_DY = TTF_RenderText_Solid(username, "KIKOWANI STATION", { 255,0,0 });
							SDL_Texture* Map_N_T = SDL_CreateTextureFromSurface(R, Map_DY);
							SDL_FreeSurface(Map_DY);

							SDL_Rect* Map_Name = new SDL_Rect();

							SDL_Rect* Player = new SDL_Rect();
							Player->w = 100; Player->h = 100; Player->x = WIDTH / 2 - 50, Player->y = HEIGHT / 2 - 50;

							SDL_Texture* Player_Pistol = IMG_LoadTexture(R, "Assets/Player.png");

							SDL_Texture* RED = IMG_LoadTexture(R, "Assets/RED.png"); //For Collider Displaying

							SDL_QueryTexture(Map_N_T, NULL, NULL, &Map_Name->w, &Map_Name->h);

							Map_Name->x = WIDTH / 2 - Map_Name->w / 2; Map_Name->y = HEIGHT / 2 - Map_Name->h / 2;

							int ammo = 26; //Reserve
							int  mag = 13; //Magazine

							Mix_PlayChannel(-1, Door_Slam, 0);

							SDL_RenderClear(R);
							SDL_RenderCopy(R, Map_N_T, NULL, Map_Name); //Display Map Name
							SDL_RenderPresent(R);
							Sleep(3000); //Wait 3 Seconds

							SDL_Rect* arr[13]; //Colliders of Kikowani Stations
							int xcoords[14]{}; //Last known x coords where movement is true
							int ycoords[14]{}; //Last known y coords where movement is true

							//Kikowani Station only
							frames = 0;

							//Collider Setting
							arr[0] = new SDL_Rect(); arr[0]->x = 228; arr[0]->y = 217; arr[0]->w = 2033 - 228; arr[0]->h = 256 - 217; //top wall
							arr[1] = new SDL_Rect(); arr[1]->x = 228; arr[1]->y = 1995; arr[1]->w = 2033 - 228; arr[1]->h = 256 - 217; //bottom wall
							arr[2] = new SDL_Rect(); arr[2]->x = 227; arr[2]->y = 220; arr[2]->w = 273 - 230; arr[2]->h = 2023 - 204; //left wall
							arr[3] = new SDL_Rect(); arr[3]->x = 1998; arr[3]->y = 220; arr[3]->w = 273 - 230; arr[3]->h = 2023 - 204; // right wall

							arr[4] = new SDL_Rect(); arr[4]->x = 600; arr[4]->y = 722; arr[4]->w = 821 - 602; arr[4]->h = 788 - 720; // top bench 1;
							arr[5] = new SDL_Rect(); arr[5]->x = 1100; arr[5]->y = 722; arr[5]->w = 821 - 602; arr[5]->h = 788 - 720; // top bench 2;
							arr[6] = new SDL_Rect(); arr[6]->x = 1580; arr[6]->y = 722; arr[6]->w = 821 - 602; arr[6]->h = 788 - 720; // top bench 3;

							arr[7] = new SDL_Rect(); arr[7]->x = 570; arr[7]->y = 1550; arr[7]->w = 821 - 602; arr[7]->h = 788 - 720; // bottom bench 1;
							arr[8] = new SDL_Rect(); arr[8]->x = 1080; arr[8]->y = 1550; arr[8]->w = 821 - 602; arr[8]->h = 788 - 720; // bottom bench 
							arr[9] = new SDL_Rect(); arr[9]->x = 1540; arr[9]->y = 1550; arr[9]->w = 821 - 602; arr[9]->h = 788 - 720; // bottom bench 3;

							arr[10] = new SDL_Rect(); arr[10]->x = 816; arr[10]->y = 1020; arr[10]->w = 1550 - 816; arr[10]->h = 1235 - 1015; // track 2;
							arr[11] = new SDL_Rect(); arr[11]->x = -60; arr[11]->y = 1020; arr[11]->w = 666; arr[11]->h = 1235 - 1015; // track 1;
							arr[12] = new SDL_Rect(); arr[12]->x = 1730; arr[12]->y = 1020; arr[12]->w = 666; arr[12]->h = 1235 - 1015; // track 3;

							//Movement Lock, 

							//Reallignment of Map
							Level_KS->x = 0; Level_KS->y = 0; Level_KS->w = 2200, Level_KS->h = 2200; //Fresh state

							//New Game, New List
							for (int i = 0; i < Shadows.length; i++)
							{
								Shadows.deleteIndex(i); //Fresh State
							}

							//IDK why we started using a new SDL_Event
							SDL_Event F;

							//Health Icon, BF Style
							SDL_Rect* H_Icon = new SDL_Rect(); H_Icon->x = WIDTH - 100, H_Icon->y = HEIGHT - 120; H_Icon->w = 64; H_Icon->h = 64;

							int h = 100;
							string H = to_string(h);

							string magz = "";
							magz.append(to_string(ammo));


							//Ammo
							SDL_Surface* Ammo_Stat = TTF_RenderText_Solid(other, magz.c_str(), { 255,255,255 });
							SDL_Texture* Ammo_num = SDL_CreateTextureFromSurface(R, Ammo_Stat);

							//Health
							SDL_Surface* Health_Stat = TTF_RenderText_Solid(other, H.c_str(), { 255,255,255 });;
							SDL_Texture* health_num = SDL_CreateTextureFromSurface(R, Health_Stat);

							SDL_Rect* D_Health = new SDL_Rect(); D_Health->x = WIDTH - 240; D_Health->y = HEIGHT - 128;
							SDL_Rect* D_Ammo = new SDL_Rect(); D_Ammo->x = WIDTH - 164; D_Ammo->y = 96;

							SDL_Surface* SHADE_H, * SHADE_SPAWN, * SHADE_D;
							SDL_Rect* Shade_H, * Shade_Spawn, * Shade_D, * R_Label;
							SDL_Texture* SH, * SD, * SS;

							SDL_Surface* Record_Label = TTF_RenderText_Solid(Record, "C", { 255,0,0 });;
							SDL_Texture* Rec_Texture = SDL_CreateTextureFromSurface(R, Record_Label);
							SDL_FreeSurface(Record_Label);



							R_Label = new SDL_Rect();
							Shade_H = new SDL_Rect();
							Shade_D = new SDL_Rect();
							Shade_Spawn = new SDL_Rect();

							SDL_QueryTexture(health_num, NULL, NULL, &D_Health->w, &D_Health->h);
							SDL_QueryTexture(Ammo_num, NULL, NULL, &D_Ammo->w, &D_Ammo->h);
							SDL_QueryTexture(Rec_Texture, NULL, NULL, &R_Label->w, &R_Label->h); R_Label->x = 40; R_Label->y = HEIGHT - 32 - R_Label->h;

							double angle = 0;
							int x = 0; int y = 0; //Crosshair Coordinates
							//SDL_Point* center = new SDL_Point(WIDTH/2, HEIGHT/2);

							SDL_Rect* CLIP = new SDL_Rect(); CLIP->w = 300; CLIP->h = 43; CLIP->x = WIDTH - 360, CLIP->y = 32;

							//SDL_RenderCopyEx(R, Player_Pistol, NULL, Player,);

							boolean reload = false;
							boolean fire = true; //Set to true after 0.3s
							boolean movement = true;
							boolean killstreak = false;

							int ks_counter = 0;
							int kills = 0;
							int cd = 0;

							int diff_h = 0;				//Difficulty of Health		
							int diff_s = FPS * 3;			//Difficulty of Spawning
							int diff_d = 0;				//Difficulty of Damage
							int diff_sp = 0;            //Difficulty of Speed [Note: Due to the small environment of Kikowani Station, Speed Modifiers have been disabled]

							Mix_PlayMusic(Gameplay, -1);

							//Kikowani Initialized

							while (play)
							{

								//Game End
								if (h < 1) {

									//Sleep(500);

									play = false;
									Mix_HaltMusic();
									SDL_Surface* Map_DY = TTF_RenderText_Solid(username, "YOU DIED", { 255,0,0 });
									SDL_Texture* Map_N_T = SDL_CreateTextureFromSurface(R, Map_DY);
									SDL_Texture* STATISTICS;
									SDL_FreeSurface(Map_DY);

									string STATS = "Kills : ";
									STATS.append(to_string(kills) + " | ");
									STATS.append(to_string(frames / FPS) + " seconds");

									Map_DY = TTF_RenderText_Solid(EXO, STATS.c_str(), { 255,255,255 });
									STATISTICS = SDL_CreateTextureFromSurface(R, Map_DY);
									SDL_Rect* D_STATS = new SDL_Rect();
									SDL_QueryTexture(STATISTICS, NULL, NULL, &D_STATS->w, &D_STATS->h);

									D_STATS->x = WIDTH / 2 - D_STATS->w / 2; D_STATS->y = HEIGHT - 320;

									SDL_FreeSurface(Map_DY);

									Mix_PlayMusic(End, 0);

									SDL_RenderClear(R);
									SDL_RenderCopy(R, STATISTICS, NULL, D_STATS);
									SDL_RenderCopy(R, Map_N_T, NULL, Map_Name);
									SDL_RenderPresent(R);
									for (int i = 0; i < Shadows.length; i++)
									{
										Shadows.deleteIndex(i);
									}
									Sleep(3000);

									//Save Relevant stats and write profile
									LOADED.kills = LOADED.kills + kills;
									LOADED.deaths++;
									LOADED.seconds_played = LOADED.seconds_played + (frames / FPS);

									write(LOADED);

									play = false;
								}

								while (SDL_PollEvent(&F)) {

									currentKeyStates = SDL_GetKeyboardState(NULL);

									if (F.type == SDL_KEYDOWN && movement)
									{
										//Partial Double Input support. Buggy and broken Movement otherwise.

										if (currentKeyStates[SDL_SCANCODE_W])
										{
											Level_KS->y = Level_KS->y + psy;
											for (int i = 0; i < 13; i++) {

												arr[i]->y = arr[i]->y + psy;
												if (SDL_HasIntersection(arr[i], Player)) {
													movement = false;
												}
											}

											if (movement) {
												for (int i = 0; i < Shadows.length; i++)
												{
													Shadows.at(i)->relocate(0, psy);
												}
											}


										}
										if (currentKeyStates[SDL_SCANCODE_S])
										{
											Level_KS->y = Level_KS->y - psy;
											for (int i = 0; i < 13; i++) {
												arr[i]->y = arr[i]->y - psy;
												if (SDL_HasIntersection(arr[i], Player)) {
													movement = false;
												}
											}

											if (movement) {
												for (int i = 0; i < Shadows.length; i++)
												{
													Shadows.at(i)->relocate(0, -psy);

												}
											}

										}
										if (currentKeyStates[SDL_SCANCODE_A])
										{
											Level_KS->x = Level_KS->x + psx;
											for (int i = 0; i < 13; i++) {
												arr[i]->x = arr[i]->x + psx;
												if (SDL_HasIntersection(arr[i], Player)) {
													movement = false;
												}
											}

											if (movement) {
												for (int i = 0; i < Shadows.length; i++)
												{
													Shadows.at(i)->relocate(psx, 0);

												}
											}
										}

										if (currentKeyStates[SDL_SCANCODE_D])
										{
											Level_KS->x = Level_KS->x - psx;
											for (int i = 0; i < 13; i++) {
												arr[i]->x = arr[i]->x - psx;
												if (SDL_HasIntersection(arr[i], Player)) {
													movement = false;
												}
											}

											if (movement) {
												for (int i = 0; i < Shadows.length; i++)
												{
													Shadows.at(i)->relocate(-psx, 0);

												}
											}

										}
										if (currentKeyStates[SDL_SCANCODE_R])
										{
											if (mag < 13 && mag>-1 && ammo > 0) {
												reload = true;
												Mix_PlayChannel(-1, Reload, 0);
											}
										}
										if (currentKeyStates[SDL_SCANCODE_ESCAPE])
										{
											play = false;
											SDL_DestroyTexture(health_num);
											Mix_HaltMusic();
											break;
										}

										if (currentKeyStates[SDL_SCANCODE_K])
										{
											h = -10;
											break;
										}


										if (movement) {
											for (int i = 0; i < 13; i++) {

												ycoords[i] = arr[i]->y; //ycoords[1] -> arr[1]->y wahan pe where movement is allowed 
												xcoords[i] = arr[i]->x;

											}

											xcoords[13] = Level_KS->x;
											ycoords[13] = Level_KS->y;

										}
									}

									if (F.type == SDL_MOUSEBUTTONUP) {
										if (F.button.button == SDL_BUTTON_LEFT) {

											for (int i = 0; i < Shadows.length; i++)
											{
												Enemy* temp = Shadows.at(i);
												if (temp->target && fire && !reload && mag > 0) {
													temp->hurt();
												}
											}

											if (fire && !reload && mag > 0) {
												Mix_PlayChannel(-1, Gunshot, 0);
												mag--;
												fire = false;
											}
											if (mag == 0) {
												Mix_PlayChannel(-1, Empty, 0);
											}


										}
									}
									break;
								}

								if (!fire) {
									if (frames % (FPS / 3) == 0) {
										cd++;
										if (cd == 1) {
											cd = 0;
											fire = true;
										}
									}
								}

								frames++;
								if (frames % (FPS / 10) == 0) {
									X++;
									if (X == 20) {
										X = 0;
									}
								}
								//	SDL_DestroyTexture(Player_Pistol);
								Player_Pistol = player_still[X];

								//Reload Timing
								if (reload) {

									fire = false;

									if (frames % (FPS / 12) == 0) {
										Y++;

										if (Y == 15) {
											Y = 0;
											reload = false;
											ammo = ammo + mag;
											if (ammo > 12) {
												mag = 13;
												ammo = ammo - 13;
											}
											else {
												mag = ammo;
												ammo = 0;
											}


										}
									}

									Player_Pistol = player_reload[Y];
								}

								frame_start = SDL_GetTicks();

								SDL_GetMouseState(&x, &y);

								angle = atan2f(y - HEIGHT / 2, x - WIDTH / 2);

								SDL_RenderClear(R);

								angle = (angle * 360) / (3.142 * 2);

								H = to_string(h);
								magz = "";
								magz.append(to_string(ammo));

								Ammo_Stat = TTF_RenderText_Solid(other, magz.c_str(), { 255,255,255 });;
								Ammo_num = SDL_CreateTextureFromSurface(R, Ammo_Stat);

								Health_Stat = TTF_RenderText_Solid(other, H.c_str(), { 255,255,255 });;
								health_num = SDL_CreateTextureFromSurface(R, Health_Stat);

								SDL_QueryTexture(health_num, NULL, NULL, &D_Health->w, &D_Health->h);
								SDL_QueryTexture(Ammo_num, NULL, NULL, &D_Ammo->w, &D_Ammo->h);

								SDL_GetMouseState(&(Crosshair->cursor.x), &(Crosshair->cursor.y));

								SDL_RenderCopy(R, Kikowani_Station, NULL, Level_KS);

								SDL_RenderCopy(R, Mag_Anim[mag], NULL, CLIP);

								SDL_RenderCopyEx(R, Player_Pistol, NULL, Player, angle, NULL, SDL_FLIP_NONE);

								SDL_RenderCopy(R, F_AID, NULL, H_Icon);

								SDL_RenderCopy(R, health_num, NULL, D_Health);
								SDL_RenderCopy(R, Ammo_num, NULL, D_Ammo);
								if (mag == 0) {
									SDL_RenderCopy(R, info_reload, NULL, show_info_reload);
								}

								for (int i = 0; i < 13; i++) {
									//Collider Rendering
									SDL_RenderCopy(R, NULL, NULL, arr[i]);

									//Renderer, Texture, Source Rect, Destination Rect
								}

								//Spawner
								//Diff_S = Spawn after these many frames //seconds

								if (frames % diff_s == 0) {

									//int sx, sy;
									int s_random = rand() % 4;

									Enemy* G;

									if (s_random == 0) { //Right Wall
										G = new Enemy(Level_KS->x, rand() % (Level_KS->h), diff_h, 0, diff_d, R);
										Shadows.insertHead(G);
									}

									if (s_random == 1) { //Left Wall
										G = new Enemy(Level_KS->w - Level_KS->x, rand() % (Level_KS->h - Level_KS->y), diff_h, 0, diff_d, R);
										Shadows.insertHead(G);
									}

									if (s_random == 2) { //Top Wall
										G = new Enemy(rand() % (Level_KS->x - Level_KS->w), Level_KS->y, diff_h, 0, diff_d, R);
										Shadows.insertHead(G);
									}

									if (s_random == 3) { //Bottom Wall
										G = new Enemy(rand() % (Level_KS->x - Level_KS->w), (Level_KS->h - Level_KS->y), diff_h, 0, diff_d, R);
										Shadows.insertHead(G);
									}

									int music_select = rand() % 12;

									if (music_select == 0) {
										Mix_PlayChannel(-1, Spawn_1, 0);
									}
									if (music_select == 1) {
										Mix_PlayChannel(-1, Spawn_2, 0);
									}
									if (music_select == 2) {
										Mix_PlayChannel(-1, Spawn_3, 0);
									}
									if (music_select == 3) {
										Mix_PlayChannel(-1, Spawn_4, 0);
									}
								}


								for (int i = 0; i < Shadows.length; i++)
								{
									Enemy* temp = Shadows.at(i);
									temp->move(WIDTH / 2, HEIGHT / 2);
									temp->in_aim(Crosshair);
									temp->draw();
									if (SDL_HasIntersection(Player, temp->BB)) {

										if (!invulnerable) {
											h = h - temp->damage;
											Mix_PlayChannel(-1, Player_Hurt, 0);
											invulnerable = true;
										}

									}
								}

								if (invulnerable) {
									I++;
									if (I == 288) {
										invulnerable = false;
										I = 0;
									}
								}

								
								if (frames % FPS < FPS / 2) {
									SDL_RenderCopy(R, Rec_Texture, NULL, R_Label);
								}

								Crosshair->Draw();

								SHADE_H = TTF_RenderText_Solid(EXO, ("H : " + to_string(diff_h)).c_str(), { 255,255,255 }); SH = SDL_CreateTextureFromSurface(R, SHADE_H);
								SHADE_D = TTF_RenderText_Solid(EXO, ("D : " + to_string(diff_d)).c_str(), { 255,255,255 }); SD = SDL_CreateTextureFromSurface(R, SHADE_D);
								SHADE_SPAWN = TTF_RenderText_Solid(EXO, ("S : " + to_string(diff_s) + "     K : " + to_string(kills)).c_str(), { 255,255,255 }); SS = SDL_CreateTextureFromSurface(R, SHADE_SPAWN);

								SDL_QueryTexture(SH, NULL, NULL, &Shade_H->w, &Shade_H->h); Shade_H->x = WIDTH / 2 - 128 - (Shade_H->w) / 2; Shade_H->y = HEIGHT - 32 - Shade_H->h;
								SDL_QueryTexture(SD, NULL, NULL, &Shade_D->w, &Shade_D->h); Shade_D->x = WIDTH / 2 - (Shade_D->w) / 2; Shade_D->y = HEIGHT - 32 - Shade_D->h;
								SDL_QueryTexture(SS, NULL, NULL, &Shade_Spawn->w, &Shade_Spawn->h); Shade_Spawn->x = WIDTH / 2 + 32 + (Shade_D->w) / 2; Shade_Spawn->y = HEIGHT - 32 - Shade_Spawn->h;

								SDL_RenderCopy(R, SH, NULL, Shade_H);
								SDL_RenderCopy(R, SD, NULL, Shade_D);
								SDL_RenderCopy(R, SS, NULL, Shade_Spawn);

								SDL_RenderPresent(R);

								SDL_FreeSurface(Ammo_Stat);
								SDL_FreeSurface(Health_Stat);
								SDL_DestroyTexture(health_num);
								SDL_DestroyTexture(Ammo_num);
								SDL_FreeSurface(SHADE_H);
								SDL_FreeSurface(SHADE_D);
								SDL_FreeSurface(SHADE_SPAWN);
								frame_time = SDL_GetTicks() - frame_start;

								if (frames % 144 == 0) {

									if (h < 100) {
										h++;
									}

								}


								if (delay > frame_time) {
									SDL_Delay(delay - frame_time);
								}

								for (int i = 0; i < Shadows.length; i++)
								{
									Enemy* temp = Shadows.at(i);

									if (!temp->alive) {
										Shadows.deleteIndex(i);
										kills++;
									}

								}

								frame_time = SDL_GetTicks() - frame_start;

								if (frames % (FPS * 30) == 0 && frames > 0) { // Random Diffculty increasing

									int random = rand() % 10; //0-9

									if (random < 5) { //0-4 //Increase spawn frequency - 50%

										diff_s = diff_s - 5;
										//diff_s = FPS*3

									}
									else if (random < 7) { //5 or 6 = 20%

										diff_h++;

									}
									else if (random < 10) { //7,8,9 = 30%
										diff_d++;
									}

									Mix_PlayChannel(-1, Devil_Laugh, 0);
								}

								//kills = 5
								//Ammo Return
								if (kills % 5 == 0 && kills > 0 && ks_counter != kills) {

									ammo = ammo + 13 + (diff_h * 5);
									Mix_PlayChannel(-1, Ammo_Restock, 0);
									ks_counter = kills;
								}

								if (!movement) { //
									for (int i = 0; i < 13; i++) {
										arr[i]->x = xcoords[i];
										arr[i]->y = ycoords[i];
									}
									Level_KS->x = xcoords[13]; Level_KS->y = ycoords[13];
									movement = true;
								}

							}

						}
						if (button[FS]->sel) {
							if (fullscreen) {
								SDL_SetWindowFullscreen(window, false);
								fullscreen = false;
								button[FS]->toggleActive();
							}
							else {
								SDL_SetWindowFullscreen(window, true);
								fullscreen = true;
								button[FS]->toggleActive();
							}
						}
					}
				}
			}

			frame_start = SDL_GetTicks();

			SDL_GetMouseState(&(MenuMouse->cursor.x), &(MenuMouse->cursor.y));

			//SDL_RenderClear;
			//SDL_RenderCopy
			//RenderCopyEx
			//RenderPresent

			SDL_RenderClear(R);

			SDL_RenderCopy(R, BG, NULL, NULL);
			SDL_RenderCopy(R, Splash, NULL, splash);

			menuUpdate();

			SDL_RenderPresent(R);

			if (delay > frame_time) {
				SDL_Delay(delay - frame_time);
			}

		}

		frame_time = SDL_GetTicks() - frame_start;

		if (delay > frame_time) {
			SDL_Delay(delay - frame_time);
		}

	}

	for (int i = 0; i < 20; i++) {
		SDL_DestroyTexture(player_still[i]);
	}

	for (int i = 0; i < 14; i++) {
		SDL_DestroyTexture(player_reload[i]);
	}

	for (int i = 0; i < 14; i++) {
		SDL_DestroyTexture(Mag_Anim[i]);
	}

	SDL_DestroyTexture(Kikowani_Station);
	SDL_DestroyTexture(BG);
	SDL_DestroyTexture(Splash);

	//delete[] player_still;
	//delete[] player_reload;
	//delete[] button;
	//delete[] profile_button;

	Mix_FreeChunk(Door_Creak);
	Mix_FreeChunk(Door_Slam);
	Mix_FreeChunk(Gunshot);
	Mix_FreeChunk(Reload);
	Mix_FreeChunk(Empty);

	SDL_RenderClear(R);

	SDL_DestroyRenderer(R);

	Mix_FreeMusic(Menu_Music);
	SDL_DestroyWindow(window);
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}



