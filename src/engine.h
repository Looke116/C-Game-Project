#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

/*
 * To Semei:
 *
 * I'm using a lot of "unsigned short int" around here because
 * most of the variables don't need to go below zero (unsigned)
 * or above 65,535 (short int). This might actually be overkill and
 * not necessary so if you don't like it or find it confusing we can
 * change back to normal "int"
 *
 * More info here if you're interested
 * https://www.tutorialspoint.com/cplusplus/cpp_data_types.htm
 *
 * P.S. I'm gonna do the entity class as an example and will leave
 * the Tank, Projectile and PowerUp classes to you, ignore the
 * rest for now.
 */

class Timer {
public:
	// Not really needed also weird behavior on my side ?idk?
	//	Timer();

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
	bool loadFromFile(char *path, SDL_Renderer *renderer);

	// Creates image from font string
	bool loadFromText(char *text, TTF_Font *font, SDL_Renderer *renderer,
			SDL_Color textColor);

	// Probably not needed but here for future reference
	// void setColorModulation(Uint8 red, Uint8 green, Uint8 blue);
	void setBlendMode(SDL_BlendMode blending);
	void setAlpha(Uint8 alpha);
	int getWidth();
	int getHeight();
	void setWidth(int w);
	void setHeight(int h);

	void draw(int x, int y, SDL_Renderer *renderer, SDL_Rect *clip, double angle, SDL_Point *center,
			SDL_RendererFlip flip);

private:
	// The actual hardware texture
	SDL_Texture *texture;

	// Image dimensions
	int width;
	int height;
};

class Button {
public:
	Button(int x, int y, int w, int h);
	~Button();

	void setPosition(int x, int y);
	void setTexture(char *path, SDL_Renderer *renderer);
	void setText(char *text, TTF_Font *font, SDL_Renderer *renderer);

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
	// Empty constructor
	Entity();

	// Custom constructor
	Entity(int x, int y, int width, int height, int screenWidth, int screenHeight);

	// Destructor
//	~Entity();

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

	// Getters and Setters
	int getGravity();
	void setGravity(int gravity);
	void setSpeed(int x, int y);
	void setSpeedX(int x);
	void setSpeedY(int y);
	SDL_Rect getBoundingBox();

	void draw(SDL_Renderer *renderer);

	// Movement and Collision Functions
	void updatePos();
	void updateSpeed();
	void handleCollision(int collisionTypem, SDL_Rect *collisionTarget);
	void move(SDL_Rect *collisionTarget);
	void move(SDL_Rect *collisionTargets[]);

protected:
	int checkCollision(const SDL_Rect *rect);

// These might need to be change to protected instead of private
private:
	short int gravity;
	short int xSpeed;
	short int ySpeed;
	short int maxSpeed;

	bool onGround;

	const unsigned short int SCREEN_WIDTH;
	const unsigned short int SCREEN_HEIGHT;

	/* Values from 100 to 0
	 * Works like a percentage of velocity transfer/loss
	 * 100 	- means speed doesn't change when ricocheting
	 * 0 		- means speed reduced to 0 (doesn't bounce at all)
	 */
	//	short int reboundForce; 	// Not used in current version
	SDL_Rect boundingBox;
};

class Tank: Entity {
public:
	Tank();
	~Tank();

private:
//	unsigned short int health;
//	unsigned short int shield;
};

class Projectile: Entity {
public:
	Projectile();
	~Projectile();

private:
	short int owner;				// Stores the id of the player that shot the projectile
//	char owner1;					// Maybe use char (1 byte) instead of short int (2 bytes)
};

class PowerUp: Entity {
public:
	PowerUp();
	~PowerUp();

private:
	// Open to ideas
	enum powerUpTypes {
		HEALTH, // Gives Health and Shield based on a fixed amount or random if skill tree is added
		WEAPONS,					// Gives between 1-3 weapons
		TELEPORT,					// Teleports the player to a random place on the map
		OVER_POWERED_BS	// idk something overpower with a low spawn chance cuz why not ;)
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
