//#include "boost/asio.hpp" 	// Networking Library -- might change later idk
#include <SDL.h> 					// Graphics Library
#include "engine.h"				// Classes and SLD wrapper

#include <iostream>

SDL_Window *window = NULL;
SDL_Surface *surface = NULL;
SDL_Renderer *renderer = NULL;

const unsigned short int SCREEN_WIDTH = 1280;
const unsigned short int SCREEN_HEIGHT = 720;

bool init();

void close();

int main(int argc, char *args[]) {

	bool quit = false;
	int floorHeight = 180;

	SDL_Rect floor = { 0, 720 - floorHeight, 1280, floorHeight };
	SDL_Event event;

	Entity companionCube { 100, 400, 50, 50, SCREEN_WIDTH, SCREEN_HEIGHT };

	if (!init()) {
		printf("Failed to initialize");
		exit(1);
	}

	while (!quit) {
		// Check to see if any even is on the queue and process it
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}

			// Get current key states and change speed depending on the key that is pressed
			const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
			if (currentKeyStates[SDL_SCANCODE_ESCAPE]) {
				quit = true;
			} else if (currentKeyStates[SDL_SCANCODE_UP] || currentKeyStates[SDL_SCANCODE_W]) {
				companionCube.setSpeedY(-20);
			} else if (currentKeyStates[SDL_SCANCODE_DOWN] || currentKeyStates[SDL_SCANCODE_S]) {
				companionCube.setSpeedY(20);
			} else if (currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_A]) {
				companionCube.setSpeedX(-10);
			} else if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_D]) {
				companionCube.setSpeedX(10);
			}
		}
//		SDL_Delay(5);

		// Clear screen
		SDL_SetRenderDrawColor(renderer, 0x99, 0x99, 0x99, 0xFF);
		SDL_RenderClear(renderer);

		// Draw the floor
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
		SDL_RenderFillRect(renderer, &floor);

		companionCube.move(&floor);
		companionCube.draw(renderer);

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

	// Create window
	window = SDL_CreateWindow("ShellShock Live -- Clone",						// Window Title
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 		// Window Position
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);				// Window Dimensions
	if (window == NULL) {
		printf("Could not create window! SDL Error: %s\n", SDL_GetError());
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
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window = NULL;
	renderer = NULL;

	SDL_Quit();
}
