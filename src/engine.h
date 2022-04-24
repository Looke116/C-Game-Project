#import <SDL.h>
#import <string>

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

class Entity {
public:
	// Empty constructor
	Entity();

	// Custom constructor
	Entity(int x, int y, int width, int height, int screenWidth, int screenHeight);

	// Destructor
	~Entity();

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
	};

	// Getters and Setters
	int getGravity();
	void setGravity(int gravity);
	void setSpeed(int x, int y);

	void draw(SDL_Renderer *renderer);
	void move(SDL_Rect *collisionTarget);

protected:
	int checkCollision(const  SDL_Rect *rect);

private:
	short int gravity;
	short int xSpeed;
	short int ySpeed;
	short int maxVel;

	unsigned short int SCREEN_WIDTH;
	unsigned short int SCREEN_HEIGHT;

	/* Values from 100 to 0
	 * Works like a percentage of velocity transfer/loss
	 * 100 	- means speed doesn't change when ricocheting
	 * 0 		- means speed reduced to 0 (doesn't bounce at all)
	 */
	short int reboundForce;

	SDL_Rect boundingBox;
};

class Tank: Entity {
public:
	Tank();
	~Tank();

private:
	unsigned short int health;
	unsigned short int shield;
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
		HEALTH, 						// Gives Health and Shield based on a fixed amount or random if skill tree is added
		WEAPONS,					// Gives between 1-3 weapons
		TELEPORT,					// Teleports the player to a random place on the map
		OVER_POWERED_BS	// idk something overpower with a low spawn chance cuz why not ;)
	};
};

class Texture {
public:
	Texture();
	~Texture();

	bool loadFromFile(std::string path);

	int getWidth();
	int getHeight();

	void render();
	void free();

private:
	SDL_Texture *texture;

	int x, y;
	int width, height;
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
