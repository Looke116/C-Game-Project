#include <cmath>
#include "engine.h"

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

Entity::Entity(int x, int y, int width, int height, int screenWidth, int screenHeight) :
		SCREEN_WIDTH(screenWidth), SCREEN_HEIGHT(screenHeight) {
	boundingBox = { x, y, width, height };
	gravity = 10;
	xSpeed = 0;
	ySpeed = 0;
	maxSpeed = 40;
	onGround = false;
	this->reboundForce = 50;
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

void Entity::draw(SDL_Renderer *renderer) {
	// Render Entity
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(renderer, &boundingBox);

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
		// TODO solve this line
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

	// TODO wall collision still not working
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

LTimer::LTimer() {
	startTicks = 0;
	pausedTicks = 0;
	started = false;
	paused = false;
}

void LTimer::start() {
	started = true;
	paused = false;

	startTicks = SDL_GetTicks();
	pausedTicks = 0;
}

void LTimer::stop() {
	started = false;
	paused = false;

	startTicks = 0;
	pausedTicks = 0;
}

void LTimer::pause() {
	if (started && !paused) {
		paused = true;

		pausedTicks = SDL_GetTicks() - startTicks;
		startTicks = 0;
	}
}

void LTimer::unpause() {
	if (started && paused) {
		paused = false;

		startTicks = SDL_GetTicks();
		pausedTicks = 0;
	}
}

void LTimer::reset() {
	started = true;
	paused = false;

	startTicks = SDL_GetTicks();
	pausedTicks = 0;
}

Uint32 LTimer::getTicks() {
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

bool LTimer::isStarted() {
	return started;
}

bool LTimer::isPaused() {
	return paused;
}
