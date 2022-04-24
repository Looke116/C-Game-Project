#import "engine.h"

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

Entity::Entity(int x, int y, int width, int height, int screenWidth, int screenHeight) {
	boundingBox = { x, y, width, height };
	gravity = 1;
	xSpeed = 0;
	ySpeed = 0;
	maxVel = 10;
	SCREEN_WIDTH = screenWidth;
	SCREEN_HEIGHT = screenHeight;
	this->reboundForce = 50;
}

Entity::~Entity() {
	/*delete gravity;
	 delete xSpeed;
	 delete ySpeed;
	 delete maxVel;
	 delete reboundForce;
	 delete boundingBox;*/

	// TODO find out how to properly implement a destructor
	// more info here https://stackoverflow.com/questions/22491174/when-do-we-need-to-define-destructors
}

int Entity::getGravity() {
	return gravity;
}

void Entity::setGravity(int gravity) {
	this->gravity = gravity;
}

void Entity::setSpeed(int x, int y) {
	xSpeed = x;
	ySpeed = y;
}

// Returns int from enum collisionDirection
int Entity::checkCollision(const SDL_Rect *rect) {

	bool topCollision = false;
	bool bottomCollision = false;
	bool leftCollision = false;
	bool rightCollision = false;

	/*
	 * Save the top left and bottom right points of the current
	 * entity and the target entity that we make the check against.
	 *
	 * By taking two opposite corners we know where the
	 * bounding box of the entity is in our coordinate system
	 */
	struct dimensions {
		int top;
		int bottom;
		int left;
		int right;
	};

	struct dimensions entity = { boundingBox.y, boundingBox.y + boundingBox.h, boundingBox.x, boundingBox.x + boundingBox.w };
	struct dimensions target = { rect->y, rect->y + rect->h, rect->x, rect->x + rect->w };

	/*
	 * Check for collision with the walls
	 */
	if (entity.left < 0) {
		return LEFT_COLLISION;
	} else if (entity.right > SCREEN_WIDTH) {
		return RIGHT_COLLISION;
	}

	/*
	 * I don't know how to explain this to make sense so read here about it
	 * https://lazyfoo.net/tutorials/SDL/27_collision_detection/index.php
	 */

	// Check for collision on the x axis
	if (entity.top <= target.bottom) {
		topCollision = true;
	} else if (entity.bottom >= target.top) {
		bottomCollision = true;
	} else if (entity.left >= target.right) {
		leftCollision = true;
	} else if (entity.right >= target.left) {
		rightCollision = true;
	}

	// Check for collision on the y axis
	if (topCollision) {
		if ((target.left < entity.left) && (entity.left < target.right)) {
			return TOP_COLLISION;
		} else if ((target.left < entity.right) && (entity.right < target.right)) {
			return TOP_COLLISION;
		}
	} else if (bottomCollision) {
		if ((target.left < entity.left) && (entity.left < target.right)) {
			return BOTTOM_COLLISION;
		} else if ((target.left < entity.right) && (entity.right < target.right)) {
			return BOTTOM_COLLISION;
		}
	} else if (leftCollision) {
		if ((target.top < entity.top) && (entity.top < target.bottom)) {
			return LEFT_COLLISION;
		} else if ((target.top < entity.bottom) && (entity.bottom < target.bottom)) {
			return LEFT_COLLISION;
		}
	} else if (rightCollision) {
		if ((target.top < entity.top) && (entity.top < target.bottom)) {
			return RIGHT_COLLISION;
		} else if ((target.top < entity.bottom) && (entity.bottom < target.bottom)) {
			return RIGHT_COLLISION;
		}
	}

	return NO_COLLISION;
}

void Entity::draw(SDL_Renderer *renderer) {
	// Render Entity
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(renderer, &boundingBox);

	// These two lines are for when we are going to add textures
//	SDL_Texture *texture = NULL;
//	SDL_RenderCopy( renderer, texture, NULL, &boundingBox );
}

void Entity::move(SDL_Rect *collisionTarget) {

	int collisionDirection = checkCollision(collisionTarget);

	// If we have a collision either up or down we reverse the speed on the y axis
	// Else if we have a collision on either side we reverse the speed on the x axis and apply gravity
	// Else just apply gravity
	if (collisionDirection == TOP_COLLISION || collisionDirection == BOTTOM_COLLISION) {
		ySpeed = -ySpeed;
	} else if (collisionDirection == LEFT_COLLISION || collisionDirection == RIGHT_COLLISION) {
		xSpeed = -xSpeed;
		ySpeed += gravity;
	} else {
		ySpeed += gravity;
	}

	// Move the entities coordinates according to the speed
	boundingBox.x += xSpeed;
	boundingBox.y += ySpeed;

	// Reduce speed over time if on ground
	if (collisionDirection == BOTTOM_COLLISION) {
		if (xSpeed > 0) {
			xSpeed--;
		} else if (xSpeed < 0) {
			xSpeed++;
		}
	}
//
//	if (ySpeed > 0) {
//		ySpeed--;
//	} else if (ySpeed < 0) {
//		ySpeed++;
//	}
}
