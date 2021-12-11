#ifndef __MODULEPLAYER_H__
#define __MODULEPLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "Box2D/Box2D/Box2D.h"
#include "List.h"

struct SDL_Texture;

class Player : public Module
{
public:
	// Constructor
	Player(bool startEnabled);

	// Destructor
	~Player();

	// Awake
	bool Awake(pugi::xml_node&);

	// Called when the module is activated
	// Loads the necessary textures for the player
	bool Start() override;

	// Called at the middle of the application loop
	// Processes new input and handles player movement
	bool Update(float dt) override;

	// Called at the end of the application loop
	// Performs the render call of the player sprite
	bool PostUpdate() override;

	// Collision callback, called when the player intersects with another collider
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB) override;

	// Save and load the position of the playerç
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	bool CleanUp() override;

public:
	// Position of the player in the map
	iPoint position;
	iPoint startPos;
	iPoint checkPointPos;
	b2Vec2* velocity;

	// player's body
	PhysBody* playerBody;
	b2Body* b;

	//add a shape
	b2CircleShape playerCircle;

	// The speed in which we move the player (pixels per frame)
	int speed;

	// Jumps of the player
	int jump;
	bool onGround;

	// The player spritesheet loaded into an SDL_Texture
	SDL_Texture* texture = nullptr;
	SDL_Texture* controlsTex;
	SDL_Texture* tutorialsTex;

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnimation = nullptr;

	// A set of animations
	Animation idleAnimR;
	Animation idleAnimL;
	Animation climbAnim;
	Animation climbDownAnim;
	Animation rightAnim;
	Animation leftAnim;
	Animation rightAnimShift;
	Animation leftAnimShift;
	Animation jumpL;
	Animation jumpR;
	Animation fallL;
	Animation fallR;

	// Tutorials animations
	Animation JumpTut;
	Animation ladderTut;
	Animation openTut;

	SDL_Rect tutRect;
	SDL_Rect chestRect;
	SDL_Rect ladderRect;
	SDL_Rect rect;

	// A flag to detect when the player has been destroyed
	int lives = 3;
	bool controlsVisible = false;
	bool tutorialVisible = false;
	bool chestFound = false;
	bool chestOpen = false;
	bool ladderClose = false;
	bool GodMode = false;
	bool checkPoint = false;
	bool checkPointReached = false;
	bool checkPointCollision = false;
	bool win = false;


	// XML files
	SString playerSprites;
	SString controls;
	SString tutorials;

	// Sound effects indices
	//uint laserFx = 0;
	//uint explosionFx = 0;
	//uint loopFx = 0;
};

#endif // __MODULEPLAYER_H__