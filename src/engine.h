#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include "common.h"
using namespace std;

class Timer {
public:
	Timer();

	void start();
	void stop();
	void pause();
	void unpause();
	void reset();

	// Gets the timer's time
	Uint32 getTicks();

	// Checks the status of the timer
	bool isStarted();
	bool isPaused();

private:
	// The clock time when the timer started
	Uint32 startTicks;

	// The ticks stored when the timer was paused
	Uint32 pausedTicks;

	// The timer status
	bool started;
	bool paused;
};

class Texture {
public:
	// Initializes variables
	Texture();

	// Deallocates memory
	~Texture();

	// Loads image from specified path
	bool loadFromFile(char *path);

	// Creates image from font string
	bool loadFromText(const char *text, SDL_Color textColor);

	// Probably not needed but here for future reference
	// void setColorModulation(Uint8 red, Uint8 green, Uint8 blue);
	void setBlendMode(SDL_BlendMode blending);
	void setAlpha(Uint8 alpha);
	int getWidth();
	int getHeight();
	void setWidth(int w);
	void setHeight(int h);
	void setRelativePos(int x, int y);

	void draw(int x, int y, SDL_Renderer *renderer, SDL_Rect *clip, double angle, SDL_Point *center,
			SDL_RendererFlip flip);

private:
	// The actual hardware texture
	SDL_Texture *texture;

	// Image dimensions
	int width;
	int height;

	int relativeX;
	int relativeY;
};

class Button {
public:
	Button(int x, int y, int w, int h);
	~Button();

	void setPosition(int x, int y);
	void setTexture(char *path);
	void setText(const char *text);

	bool pressed(SDL_Event *e);

	void draw(SDL_Renderer *renderer);

private:
	const unsigned short int BUTTON_WIDTH;
	const unsigned short int BUTTON_HEIGHT;

	bool isPressed;
	SDL_Point position;
	Texture texture;
};

class Entity {
public:
	Entity();

	enum collisionDirection {
		NO_COLLISION,
		TOP_COLLISION,
		BOTTOM_COLLISION,
		LEFT_COLLISION,
		RIGHT_COLLISION,
		TOP_LEFT_COLLISION,
		TOP_RIGHT_COLLISION,
		BOTTOM_LEFT_COLLISION,
		BOTTOM_RIGHT_COLLISION,
		RIGHT_WALL_COLLISION,
		LEFT_WALL_COLLISION,
		ALL_SIDES,
		OUT_OF_BOUNDS,
	};

	Timer timer;
	Texture *texture;

	// Getters and Setters
	int getGravity();
	void setGravity(int gravity);
	void setSpeed(int x, int y);
	void setSpeedX(int x);
	void setSpeedY(int y);
	void setBoundingBox(int x, int y, int w, int h);
	SDL_Rect getBoundingBox();

	void draw(SDL_Renderer *renderer);

	// Movement functions
	void move(SDL_Rect *collisionTarget);
	void move(vector<SDL_Rect> collisionTargets);

	enum direction {
		UP, DOWN, LEFT, RIGHT,
	};

protected:
	short int gravity;
	short int xSpeed;
	short int ySpeed;
	short int maxSpeed;

	bool onGround;

	SDL_Rect boundingBox;

	int checkCollision(const SDL_Rect *rect);

// These might need to be change to protected instead of private
private:
	/* Values from 100 to 0
	 * Works like a percentage of velocity transfer/loss
	 * 100 	- means speed doesn't change when ricocheting
	 * 0 		- means speed reduced to 0 (doesn't bounce at all)
	 */
	//	short int reboundForce; 	// Not used in current version
	// Movement and collision functions
	void updatePos();
	void updateSpeed();
	void handleCollision(int collisionType, SDL_Rect *collisionTarget);
};

class Projectile: public Entity {
public:
	Projectile(int x, int y, int direction);

	bool move(SDL_Rect *collisionTarget);
	bool move(vector<SDL_Rect> collisionTargets);

private:
	unsigned short int direction;

	void updatePos();
	void updateSpeed();
	void handleCollision(int collisionType, SDL_Rect *collisionTarget);

//	static Texture texture;
};

class Tank: public Entity {
public:
	Tank();

	void setAI(bool);
	void callAI();
	void shoot();
	void handleInput(const Uint8 *keyStates, bool secondPlayer = true);
	void draw(SDL_Renderer *renderers);
	void move(SDL_Rect *collisionTarget);
	void move(vector<SDL_Rect> collisionTargets);

	void setDamage();

	enum color {
		GREEN, ORANGE, RED,
	};

private:
	bool isEnemy;
	bool isDead;
	bool isAI;

	unsigned short int health;
	unsigned short int direction;

//	Projectile *projectiles;
	vector<Projectile> projectiles;
};

class PowerUp: Entity {
public:
	PowerUp();

private:
	// Open to ideas
	enum powerUpTypes {
		HEALTH, // Gives Health and Shield based on a fixed amount or random if skill tree is added
		WEAPONS,					// Gives between 1-3 weapons
		TELEPORT,					// Teleports the player to a random place on the map
		OVER_POWERED_BS	// idk something overpowered with a low spawn chance cuz why not ;)
	};
};

/* TODO Mesh class
 * https://math.okstate.edu/people/yqwang/teaching/math6590_fall09/demo/mesh.pdf
 *	https://stackoverflow.com/questions/62710399/2d-mesh-class-implementation-in-c
 *
 *	https://stackoverflow.com/questions/37501231/sdl2-draw-and-fill-shapes
 *
 *	class Mesh{
 *	public:
 *		Mesh();
 *		~Mesh();
 *	};
 */
