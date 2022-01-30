#ifndef __MODULEPLAYER_H__
#define __MODULEPLAYER_H__

#include "Entity.h"
#include "Animation.h"
#include "Point.h"
#include "Box2D/Box2D/Box2D.h"
#include "List.h"

struct SDL_Texture;

class Player : public Entity
{
public:
	// Constructor
	Player(iPoint position_);

	// Destructor
	~Player();

	// Awake
	//bool Awake(pugi::xml_node&);

	// Called at the middle of the application loop
	// Processes new input and handles player movement
	bool Update(float dt) override;

	// Save and load the position of the playerç
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	bool CleanUp() override;

public:
	// Position of the player in the map
	iPoint checkPointPos;

	// player's body
	PhysBody* sensor;
	b2Body* b;

	//add a shape
	b2PolygonShape playerSensor;

	int  playerHitbox[16] = {
		0, 2,
		0, 24,
		2, 26,
		24, 26,
		26, 24,
		26, 2,
		24, 0,
		2, 0
	};

	// The speed in which we move the player (pixels per frame)
	int speed;

	// Score
	int points;

	// Jumps of the player
	int jump;
	bool onGround;

	// The player spritesheet loaded into an SDL_Texture
	SDL_Texture* texture = nullptr;
	SDL_Texture* controlsTex;
	SDL_Texture* tutorialsTex;

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction

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
	SDL_Rect coinRect;
	SDL_Rect rect;

	// A flag to detect when the player has been destroyed
	int lives = 3;
	bool keyFound = false;
	bool chestOpen = false;
	bool ladderClose = false;
	bool GodMode = false;
	bool checkPoint = false;
	bool checkPointReached = false;
	bool checkPointCollision = false;
	bool playerHit = false;
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