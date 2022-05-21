//#include "boost/asio.hpp" 	// Networking Library -- might change later idk
#include <SDL.h> 					// Graphics Library
#include <SDL_image.h>		// Extension Library for using images/textures
#include <SDL_ttf.h>				// Extension Library for using text with SDL
#include "engine.h"				// Classes and SLD wrapper
#include <iostream>
//#define debug

SDL_Window *window = NULL;
SDL_Surface *surface = NULL;
SDL_Renderer *renderer = NULL;

TTF_Font *font;

const unsigned short int SCREEN_WIDTH = 1280;
const unsigned short int SCREEN_HEIGHT = 720;

const unsigned short int BUTTON_WIDTH = 200;
const unsigned short int BUTTON_HEIGHT = 100;

enum gameState {
	MAIN_MENU, LOBBY, MATCH,
};

bool init();

void close();

int main(int argc, char *args[]) {

	if (!init()) {
		printf("Failed to initialize");
		exit(1);
	}

	bool running = true;
	int currentState = MAIN_MENU;
	SDL_Event event;

	Button startButton = { SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, 100, BUTTON_WIDTH, BUTTON_HEIGHT };
	Button quitButton = { SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, 500, BUTTON_WIDTH, BUTTON_HEIGHT };

	startButton.setText("start", font, renderer);
	quitButton.setText("quit", font, renderer);

	int floorHeight = 180;

	SDL_Rect floor = { 0, 720 - floorHeight, 1280, floorHeight };
	Entity companionCube { 100, 400, 50, 50, SCREEN_WIDTH, SCREEN_HEIGHT };

#if defined(debug)
	SDL_Delay(5);
#else
	while (running) {
		// Check to see if any even is on the queue and process it
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				running= false;
			}

			// Get current key states and change speed depending on the key that is pressed
			const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
			if (currentKeyStates[SDL_SCANCODE_ESCAPE]) {
				running = false;
			}

			if (currentState == MAIN_MENU) {
				if (quitButton.pressed(&event) == true) {
					running = false;
				} else if (startButton.pressed(&event) == true) {
					currentState = MATCH;
				}
			} else if (currentState == MATCH) {
				if (currentKeyStates[SDL_SCANCODE_UP] || currentKeyStates[SDL_SCANCODE_W]) {
					companionCube.setSpeedY(-20);
				} else if (currentKeyStates[SDL_SCANCODE_DOWN]
						|| currentKeyStates[SDL_SCANCODE_S]) {
					companionCube.setSpeedY(20);
				} else if (currentKeyStates[SDL_SCANCODE_LEFT]
						|| currentKeyStates[SDL_SCANCODE_A]) {
					companionCube.setSpeedX(-10);
				} else if (currentKeyStates[SDL_SCANCODE_RIGHT]
						|| currentKeyStates[SDL_SCANCODE_D]) {
					companionCube.setSpeedX(10);
				}
			}
		}
#endif


		// Clear screen
		SDL_SetRenderDrawColor(renderer, 0x99, 0x99, 0x99, 0xFF);
		SDL_RenderClear(renderer);

		if(currentState == MAIN_MENU){
			startButton.draw(renderer);
			quitButton.draw(renderer);
		}else if(currentState == LOBBY){
			currentState= MAIN_MENU;
		}else if(currentState == MATCH){

			// Draw the floor
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
			SDL_RenderFillRect(renderer, &floor);

			companionCube.move(&floor);
			companionCube.draw(renderer);
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
	window = SDL_CreateWindow("ShellShock Live -- Clone",						// Window Title
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 		// Window Position
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);				// Window Dimensions
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
