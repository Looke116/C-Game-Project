//#include "boost/asio.hpp" 	// Networking Library -- might change later idk
#include <SDL.h> 					// Graphics Library
#include <SDL_image.h>		// Extension Library for using images/textures
#include <SDL_ttf.h>				// Extension Library for using text with SDL
#include "engine.h"				// Classes and SLD wrapper
#include <iostream>
#include "common.h"
using namespace std;
//#define debug

// Initializing variables from common.h
SDL_Window *window = NULL;
SDL_Surface *surface = NULL;
SDL_Renderer *renderer = NULL;
TTF_Font *font = NULL;

const unsigned short int SCREEN_WIDTH = 1280;
const unsigned short int SCREEN_HEIGHT = 720;

const unsigned short int BUTTON_WIDTH = 200;
const unsigned short int BUTTON_HEIGHT = 100;

enum gameState {
	MAIN_MENU,
	SINGLEPLAYER_LEVEL_SELECT,
	SPLITSCREEN_LEVEL_SELECT,
	SINGLEPLAYER_LEVEL1,
	SINGLEPLAYER_LEVEL2,
	SINGLEPLAYER_LEVEL3,
	SINGLEPLAYER_LEVEL4,
	SINGLEPLAYER_LEVEL5,
	SINGLEPLAYER_LEVEL_BOSS,
	SPLITSCREEN_LEVEL1,
	SPLITSCREEN_LEVEL2,
	SPLITSCREEN_LEVEL3,
	SPLITSCREEN_LEVEL4,
	SPLITSCREEN_LEVEL5,
	SPLITSCREEN_LEVEL_BOSS,
	TEST,
};

bool init();
void handleEvent(SDL_Event*, int);
void drawLevel(int);
void close();

int main(int argc, char *args[]) {

	if (!init()) {
		printf("Failed to initialize");
		exit(1);
	}

	bool running = true;
	int currentState = MAIN_MENU;
	SDL_Event event;

	Button singlePlayerButton = { SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, 100, BUTTON_WIDTH,
			BUTTON_HEIGHT };
	Button splitScreenButton = { SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, 250, BUTTON_WIDTH,
			BUTTON_HEIGHT };
	Button quitButton = { SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, 500, BUTTON_WIDTH, BUTTON_HEIGHT };
	Button backButton = { SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT, BUTTON_WIDTH,
			BUTTON_HEIGHT };
	Button debugButton = { 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT };

	Button lvl1 = { SCREEN_WIDTH / 2 / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2 / 2
			- BUTTON_HEIGHT / 2, BUTTON_WIDTH, BUTTON_HEIGHT };
	Button lvl2 = { SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2 / 2 - BUTTON_HEIGHT / 2,
			BUTTON_WIDTH, BUTTON_HEIGHT };
	Button lvl3 = { SCREEN_WIDTH / 2 + SCREEN_WIDTH / 2 / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2
			/ 2 - BUTTON_HEIGHT / 2, BUTTON_WIDTH, BUTTON_HEIGHT };
	Button lvl4 = { SCREEN_WIDTH / 2 / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2 - BUTTON_HEIGHT / 2,
			BUTTON_WIDTH, BUTTON_HEIGHT };
	Button lvl5 = { SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2 - BUTTON_HEIGHT / 2,
			BUTTON_WIDTH, BUTTON_HEIGHT };
	Button boss = { SCREEN_WIDTH / 2 + SCREEN_WIDTH / 2 / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2
			- BUTTON_HEIGHT / 2, BUTTON_WIDTH, BUTTON_HEIGHT };

	singlePlayerButton.setText("Singleplayer");
	splitScreenButton.setText("Local Coop");
	quitButton.setText("Quit");
	backButton.setText("Back");
	debugButton.setText("Debug");

	lvl1.setText("Level 1");
	lvl2.setText("Level 2");
	lvl3.setText("Level 3");
	lvl4.setText("Level 4");
	lvl5.setText("Level 5");
	boss.setText("Boss");

	int floorHeight = 180;

	SDL_Rect floor = { 0, 720 - floorHeight, 1280, floorHeight }; // @suppress("Invalid arguments")
	Entity companionCube;
	Tank player1;
	Tank player2;

	player1.setBoundingBox(100, 100, 50, 50);
	player2.setBoundingBox(600, 100, 50, 50);
	//	player2.setAI(true); // TODO

	vector<SDL_Rect> collisionTargets;
	collisionTargets.push_back(floor);
	collisionTargets.push_back(player1.getBoundingBox());
	collisionTargets.push_back(player2.getBoundingBox());// TODO this might create a bug later

#if defined(debug)
	SDL_Delay(5);
#else
	while (running) {
		// Check to see if any even is on the queue and process it
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				running = false;
			}

			// Get current key states and change speed depending on the key that is pressed
			const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
			if (currentKeyStates[SDL_SCANCODE_ESCAPE]) {
				running = false;
			}

			if (currentState == MAIN_MENU) {
				if (quitButton.pressed(&event)) {
					running = false;
				} else if (singlePlayerButton.pressed(&event)) {
					currentState = SINGLEPLAYER_LEVEL_SELECT;
				} else if (splitScreenButton.pressed(&event)) {
					currentState = SPLITSCREEN_LEVEL_SELECT;
				} else if (debugButton.pressed(&event)) {
					currentState = TEST;
				}
			} else if (currentState == SINGLEPLAYER_LEVEL_SELECT) {
				if (lvl1.pressed(&event)) {
					currentState = SINGLEPLAYER_LEVEL1;
				} else if (lvl2.pressed(&event)) {
					currentState = SINGLEPLAYER_LEVEL2;
				} else if (lvl3.pressed(&event)) {
					currentState = SINGLEPLAYER_LEVEL3;
				} else if (lvl4.pressed(&event)) {
					currentState = SINGLEPLAYER_LEVEL4;
				} else if (lvl5.pressed(&event)) {
					currentState = SINGLEPLAYER_LEVEL5;
				} else if (boss.pressed(&event)) {
					currentState = SINGLEPLAYER_LEVEL_BOSS;
				} else if (backButton.pressed(&event)) {
					currentState = MAIN_MENU;
				}
			} else if (currentState == SPLITSCREEN_LEVEL_SELECT) {
				if (lvl1.pressed(&event)) {
					currentState = SPLITSCREEN_LEVEL1;
				} else if (lvl2.pressed(&event)) {
					currentState = SPLITSCREEN_LEVEL2;
				} else if (lvl3.pressed(&event)) {
					currentState = SPLITSCREEN_LEVEL3;
				} else if (lvl4.pressed(&event)) {
					currentState = SPLITSCREEN_LEVEL4;
				} else if (lvl5.pressed(&event)) {
					currentState = SPLITSCREEN_LEVEL5;
				} else if (boss.pressed(&event)) {
					currentState = SPLITSCREEN_LEVEL_BOSS;
				} else if (backButton.pressed(&event)) {
					currentState = MAIN_MENU;
				}
			} else if (currentState == TEST) {
				player1.handleInput(currentKeyStates, true);
				player2.handleInput(currentKeyStates, false);
				if (backButton.pressed(&event)) {
					currentState = MAIN_MENU;
				}
			} else {
				handleEvent(&event, currentState);
				if (backButton.pressed(&event)) {
					currentState = MAIN_MENU;
				}
			}
		}
#endif

		// Clear screen
		SDL_SetRenderDrawColor(renderer, 0x99, 0x99, 0x99, 0xFF);
		SDL_RenderClear(renderer);

		if (currentState == MAIN_MENU) {
			singlePlayerButton.draw(renderer);
			quitButton.draw(renderer);
		} else if (currentState == SINGLEPLAYER_LEVEL_SELECT) {
			lvl1.draw(renderer);
			lvl2.draw(renderer);
			lvl3.draw(renderer);
			lvl4.draw(renderer);
			lvl5.draw(renderer);
			boss.draw(renderer);
			backButton.draw(renderer);
		} else if (currentState == SPLITSCREEN_LEVEL_SELECT) {
			lvl1.draw(renderer);
			lvl2.draw(renderer);
			lvl3.draw(renderer);
			lvl4.draw(renderer);
			lvl5.draw(renderer);
			boss.draw(renderer);
			backButton.draw(renderer);
		} else if (currentState == TEST) {

			// Draw the floor
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
			SDL_RenderFillRect(renderer, &floor);

			player1.move(&floor);
			player1.draw(renderer);

			player2.move(&floor);
			player2.draw(renderer);

			backButton.draw(renderer);
		} else {
			drawLevel(currentState);
			backButton.draw(renderer);
		}

		// Update screen
		SDL_RenderPresent(renderer);
	}

	close();

	return 0;
}

bool init() {

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Could not initialize! SLD Error: %s\n", SDL_GetError());
		return false;
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		printf("Could not initialize! SDL_img Error: %s\n", IMG_GetError());
		return false;
	}

	if (TTF_Init() == -1) {
		printf("Could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}

// Create window
	window = SDL_CreateWindow("ShellShock Live -- Clone",		// Window Title
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, // Window Position
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);	// Window Dimensions
	if (window == NULL) {
		printf("Could not create window! SDL Error: %s\n", SDL_GetError());
		return false;
	} else {
		surface = SDL_GetWindowSurface(window);
	}

// Load Font
	font =
			TTF_OpenFont(
					"/home/alex/Programs/eclipse-workspace/CPP-Game/resources/fonts/black-jack-font/BlackJack.ttf",
					100);
//	font = TTF_OpenFont("../resources/fonts/black-jack-font/BlackJack.ttf", 16);
	if (font == NULL) {
		printf("Could not load font! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}

// Create renderer for window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		printf("Could not create renderer! SDL Error: %s\n", SDL_GetError());
		return false;
	} else {
		// Initialize renderer color
		SDL_SetRenderDrawColor(renderer, 0x99, 0x99, 0x99, 0xFF);
	}

	return true;
}

void handleEvent(SDL_Event *event, int currentState) {

}

void drawLevel(int level) {

	if (level == SINGLEPLAYER_LEVEL1) {
		// TODO Level design
	} else if (level == SINGLEPLAYER_LEVEL2) {
	} else if (level == SINGLEPLAYER_LEVEL3) {
	} else if (level == SINGLEPLAYER_LEVEL4) {
	} else if (level == SINGLEPLAYER_LEVEL5) {
	} else if (level == SINGLEPLAYER_LEVEL_BOSS) {
	} else if (level == SINGLEPLAYER_LEVEL1) {
	} else if (level == SINGLEPLAYER_LEVEL2) {
	} else if (level == SINGLEPLAYER_LEVEL3) {
	} else if (level == SINGLEPLAYER_LEVEL4) {
	} else if (level == SINGLEPLAYER_LEVEL5) {
	} else if (level == SINGLEPLAYER_LEVEL_BOSS) {

	}
}

void close() {
	TTF_CloseFont(font);
	font = NULL;

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window = NULL;
	renderer = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
