#include <SDL_image.h>			// Extension Library for using images/textures
#include <SDL_ttf.h>
//#include <cmath>
#include <cstring>
#include "engine.h"

#include "common.h"

//Timer::Timer() {*/
//	startTicks = 0;
//	pausedTicks = 0;
//	started = false;
//	paused = false;
//}

void Timer::start() {
	started = true;
	paused = false;

	startTicks = SDL_GetTicks();
	pausedTicks = 0;
}

void Timer::stop() {
	started = false;
	paused = false;

	startTicks = 0;
	pausedTicks = 0;
}

void Timer::pause() {
	if (started && !paused) {
		paused = true;

		pausedTicks = SDL_GetTicks() - startTicks;
		startTicks = 0;
	}
}

void Timer::unpause() {
	if (started && paused) {
		paused = false;

		startTicks = SDL_GetTicks();
		pausedTicks = 0;
	}
}

void Timer::reset() {
	started = true;
	paused = false;

	startTicks = SDL_GetTicks();
	pausedTicks = 0;
}

Uint32 Timer::getTicks() {
	Uint32 time;
	if (started) {
		if (paused) {
			time = pausedTicks;
		} else {
			time = SDL_GetTicks() - startTicks;
		}
	}
	return time;
}

bool Timer::isStarted() {
	return started;
}

bool Timer::isPaused() {
	return paused;
}

Texture::Texture() {
	texture = NULL;
	width = 0;
	height = 0;
}

Texture::~Texture() {
	if (texture != NULL)
		SDL_DestroyTexture(texture);
}

bool Texture::loadFromFile(char *path) {

	// Get rid of previous texture
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
		texture = NULL;
		width = 0;
		height = 0;
	}

	// Final texture
	SDL_Texture *texture = NULL;

	// Load image at specified path
	SDL_Surface *surface = IMG_Load(path);
	if (surface == NULL) {
		printf("Unable to load image %s! SDL_Image Error: %s\n", path, IMG_GetError());
	} else {
		// Color keying - not sure if needed
		// It makes all pixels with a certain color transparent
		// SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0xFF, 0xFF));

		texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (texture == NULL) {
			printf("Unable to create texture from %s! SDL_Error: %s\n", path, SDL_GetError());
		} else {
			// Get image dimensions
			width = surface->w;
			height = surface->h;
		}

		// Get rid of old surface
		SDL_FreeSurface(surface);
	}

	this->texture = texture;
	return texture != NULL;
}

bool Texture::loadFromText(const char *text, SDL_Color textColor = { 255, 255, 255 }) {

	// Get rid of previous texture
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
		texture = NULL;
		width = 0;
		height = 0;
	}

	// Render text surface
	SDL_Surface *surface = TTF_RenderText_Solid(font, text, textColor);
	if (surface == NULL) {
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	} else {
		// Create texture from surface pixels
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (texture == NULL) {
			printf("Unable to create texture from rendered text! SDL_Error: %s\n", SDL_GetError());
		} else {
//			width = surface->w;
//			height = surface->h;
		}

		// Get rid of old surface
		SDL_FreeSurface(surface);
	}
	return texture != NULL;
}

void Texture::setBlendMode(SDL_BlendMode blending) {
	SDL_SetTextureBlendMode(texture, blending);
}

void Texture::setAlpha(Uint8 alpha) {
	SDL_SetTextureAlphaMod(texture, alpha);
}

int Texture::getWidth() {
	return width;
}

int Texture::getHeight() {
	return height;
}

void Texture::setWidth(int w) {
	width = w;
}

void Texture::setHeight(int h) {
	height = h;
}

void Texture::draw(int x, int y, SDL_Renderer *renderer, SDL_Rect *clip = NULL, double angle = 0.0,
		SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE) {

	// I don't understand what's wrong here
	// The IDE says something is wrong but it works ¯\_(ツ)_/¯
	SDL_Rect renderPos = { x, y, width, height }; // @suppress("Invalid arguments")

	if (clip != NULL) {
		renderPos.w = clip->w;
		renderPos.h = clip->h;
	}

	SDL_RenderCopyEx(renderer, texture, clip, &renderPos, angle, center, flip);
}

/*
 Entity::Entity() {
 boundingBox = { 0, 0, 0, 0 };
 gravity = NULL;
 xSpeed = 0;
 ySpeed = 0;
 maxVel = 10;
 reboundForce = NULL;
 }
 */

Button::Button(int x, int y, int w, int h) :
		BUTTON_WIDTH(w), BUTTON_HEIGHT(h) {
	position.x = x;
	position.y = y;
	isPressed = false;
	texture.setWidth(w);
	texture.setHeight(h);
}

Button::~Button() {
	texture.~Texture();
}

void Button::setPosition(int x, int y) {
	position.x = x;
	position.y = y;
}

void Button::setTexture(char *path) {
	texture.loadFromFile(path);
}

void Button::setText(const char *text) {
	texture.loadFromText(text);
}

bool Button::pressed(SDL_Event *e) {
	if (e->type == SDL_MOUSEBUTTONDOWN) {
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		// We assume the button was pressed
		isPressed = true;
		// And used the separating axis theorem to check
		//Mouse is left of the button
		if (mouseX < position.x) {
			isPressed = false;
		}
		//Mouse is right of the button
		else if (mouseX > position.x + BUTTON_WIDTH) {
			isPressed = false;
		}
		//Mouse above the button
		else if (mouseY < position.y) {
			isPressed = false;
		}
		//Mouse below the button
		else if (mouseY > position.y + BUTTON_HEIGHT) {
			isPressed = false;
		}
	}

	if (e->type == SDL_MOUSEBUTTONUP && isPressed) {
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		// We assume the mouse is within the button's bounds
		bool inside = true;
		// And used the separating axis theorem to check
		//Mouse is left of the button
		if (mouseX < position.x) {
			inside = false;
		}
		//Mouse is right of the button
		else if (mouseX > position.x + BUTTON_WIDTH) {
			inside = false;
		}
		//Mouse above the button
		else if (mouseY < position.y) {
			inside = false;
		}
		//Mouse below the button
		else if (mouseY > position.y + BUTTON_HEIGHT) {
			inside = false;
		}

		if (inside) {
			return true;
		} else {
			isPressed = false;
		}
	}
	return false;
}

void Button::draw(SDL_Renderer *renderer) {
	texture.draw(position.x, position.y, renderer);

	SDL_Rect outline;
	outline.x = position.x;
	outline.y = position.y;
	outline.w = texture.getWidth();
	outline.h = texture.getHeight();

	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(renderer, &outline);
}

Entity::Entity() {
	gravity = 10;
	xSpeed = 0;
	ySpeed = 0;
	maxSpeed = 40;
	onGround = false;
	texture = NULL;
//	this->reboundForce = 50;
}

/*Entity::~Entity() {
 delete gravity;
 delete xSpeed;
 delete ySpeed;
 delete maxVel;
 delete reboundForce;
 delete boundingBox;

 // TODO find out how to properly implement a destructor
 // more info here https://stackoverflow.com/questions/22491174/when-do-we-need-to-define-destructors
 }*/

int Entity::getGravity() {
	return gravity;
}

void Entity::setGravity(int gravity) {
	this->gravity = gravity;
}

void Entity::setSpeed(int x, int y) {
	if (x > maxSpeed) {
		if (x > 0)
			x = maxSpeed;
		else
			x = -maxSpeed;
	}

	if (abs(y) > maxSpeed) {
		if (y > 0)
			y = maxSpeed;
		else
			y = -maxSpeed;
	}

	xSpeed = x;
	ySpeed = y;

	if (y < 0) {
		onGround = false;
	}
}

void Entity::setSpeedX(int x) {
	if (abs(x) > maxSpeed) {
		if (x > 0)
			x = maxSpeed;
		else
			x = -maxSpeed;
	}

	xSpeed = x;
}

void Entity::setSpeedY(int y) {
	if (abs(y) > maxSpeed)
		if (y > 0)
			y = maxSpeed;
		else
			y = -maxSpeed;
	else
		onGround = false;

	ySpeed = y;
}

void Entity::setBoundingBox(int x, int y, int w, int h) {
	boundingBox = { x, y, w, h };
}

void Entity::draw(SDL_Renderer *renderer) {
	// Render Entity
	if(texture == NULL){
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(renderer, &boundingBox);
	}else{
		texture->draw(boundingBox.x, boundingBox.y,renderer);
	}
	printf("Pos x:%d   y:%d\n", boundingBox.x, boundingBox.y);
	printf("Speed x:%d   y:%d\n\n", xSpeed, ySpeed);
	// These two lines are for when we are going to add textures
	//	SDL_Texture *texture = NULL;
	//	SDL_RenderCopy( renderer, texture, NULL, &boundingBox );
}

void Entity::updatePos() {
	// Update x
	boundingBox.x += xSpeed;

	// Update y if the entity is in the air or the speed on the y axis points upwards
	// ySpeed < 0 		because y grows downwards (top of the screen <-> y = 0)
	if (!onGround || ySpeed < 0) {
		boundingBox.y += ySpeed;
	}
}

void Entity::updateSpeed() {
// Decrement the speed on X axis by 1 if the entity is on the ground
// Increment the speed on Y axis by 1 if current speed is under maxSpeed
	if (onGround) {
		if (xSpeed > 0) {
			xSpeed--;
		} else if (xSpeed < 0) {
			xSpeed++;
		}
	} else {
		if (ySpeed < maxSpeed) {
			ySpeed++;
		}
	}
}

// TODO
void Entity::handleCollision(int collisionType, SDL_Rect *collisionTarget) {
	if (collisionType == TOP_COLLISION) {
		ySpeed -= ySpeed;
	} else if (collisionType == BOTTOM_COLLISION) {
		onGround = true;
		ySpeed = 0;
		// TODO maths broken??
		//boundingBox.y = boundingBox.y - ((boundingBox.y + boundingBox.h) - collisionTarget->x);
	} else if (collisionType == LEFT_COLLISION || collisionType == RIGHT_COLLISION) {
		xSpeed -= xSpeed;
	} else if (collisionType == LEFT_WALL_COLLISION) {
		boundingBox.x = 0;
		xSpeed = -xSpeed;
	} else if (collisionType == RIGHT_WALL_COLLISION) {
		boundingBox.x = SCREEN_WIDTH - boundingBox.w;
		xSpeed = -xSpeed;
	} else if (collisionType == OUT_OF_BOUNDS) {
		boundingBox.x = 100;
		boundingBox.y = 100;
	}
}

void Entity::move(SDL_Rect *collisionTarget) {
	if (timer.isStarted()) {
		if (timer.getTicks() > 10) {
			// Update Position of entity
			updatePos();
			int collisionDirection = checkCollision(collisionTarget);
			if (collisionDirection != NO_COLLISION)
				handleCollision(collisionDirection, collisionTarget);
			updateSpeed();
			timer.reset();
		}
	} else {
		timer.start();
	}
}

void Entity::move(SDL_Rect *collisionTargets[]) {
	if (timer.isStarted()) {
		if (timer.getTicks() > 10) {
			// Update Position of entity
			updatePos();
			for (int i = 0; i < sizeof(collisionTargets); i++) {
				int collisionDirection = checkCollision(collisionTargets[i]);
				if (collisionDirection != NO_COLLISION)
					handleCollision(collisionDirection, collisionTargets[i]);
			}
			updateSpeed();
			timer.reset();
		}
	} else {
		timer.start();
	}
}

int Entity::checkCollision(const SDL_Rect *rect) {

	bool top = false;
	bool bottom = false;
	bool left = false;
	bool right = false;

	struct dimensions {
		int top;
		int bottom;
		int left;
		int right;
	};

	struct dimensions entity = { boundingBox.y, boundingBox.y + boundingBox.h, boundingBox.x,
			boundingBox.x + boundingBox.w };
	struct dimensions target = { rect->y, rect->y + rect->h, rect->x, rect->x + rect->w };

// Check for collision with the walls
	if (entity.bottom > SCREEN_HEIGHT) {
		return BOTTOM_COLLISION;
	} else if (entity.left < -1) {
		return LEFT_WALL_COLLISION;
	} else if (entity.right > SCREEN_WIDTH + 1) {
		return RIGHT_WALL_COLLISION;
	}

// Separating Axis Theorem
	if ((entity.bottom <= target.top) || (entity.top >= target.bottom)
			|| (entity.right <= target.left) || (entity.left >= target.right)) {

		// TODO maybe change the frequency of this check
		// Check if entity is out of bounds
		if ((entity.top > SCREEN_HEIGHT) || (entity.right < 0) || (entity.left > SCREEN_WIDTH))
			return OUT_OF_BOUNDS;

		return NO_COLLISION;
	}

// Check collision on all sides
	if (entity.top >= target.bottom) {
		top = true;
	}
	if (entity.bottom >= target.top) {
		bottom = true;
	}
	if (entity.left <= target.right) {
		left = true;
	}
	if (entity.right >= target.left) {
		right = true;
	}

	if ((top && bottom) && (left && right))
		return ALL_SIDES;

	if (top && left && right)
		return TOP_COLLISION;

	if (bottom && left && right)
		return BOTTOM_COLLISION;

	if (left && top && bottom)
		return LEFT_COLLISION;

	if (right && top && bottom)
		return RIGHT_COLLISION;

	if (top) {
		if (left) {
			return TOP_LEFT_COLLISION;
		} else if (right) {
			return TOP_RIGHT_COLLISION;
		}
	}

	if (bottom) {
		if (left) {
			return BOTTOM_LEFT_COLLISION;
		} else if (right) {
			return BOTTOM_RIGHT_COLLISION;
		}
	}

	return NO_COLLISION;
}

Tank::Tank() {
	health = 100;
	armor = 50;
	isEnemy = false;
	isAI = false;
}

void Tank::setAI(bool b) {
	isAI = b;
}

void Tank::callAI() {
//	shoot();
}

void Tank::shoot() {

}

void Tank::handleInput(const Uint8 *keyStates, bool player1 = true) {
	if (player1) {
		if (keyStates[SDL_SCANCODE_D]) {
			setSpeed(10, 0);
		} else if (keyStates[SDL_SCANCODE_A]) {
			setSpeed(-10, 0);
		} else if (keyStates[SDL_SCANCODE_W] && onGround) {
			setSpeed(0, -10);
		}else if(keyStates[SDL_SCANCODE_SPACE]){
			shoot();
		}
	}else{
		if (keyStates[SDL_SCANCODE_RIGHT]) {
			setSpeed(10, 0);
		} else if (keyStates[SDL_SCANCODE_LEFT]) {
			setSpeed(-10, 0);
		} else if (keyStates[SDL_SCANCODE_UP] && onGround) {
			setSpeed(0, -10);
		}else if(keyStates[SDL_SCANCODE_RETURN]){
			shoot();
		}
	}
}
