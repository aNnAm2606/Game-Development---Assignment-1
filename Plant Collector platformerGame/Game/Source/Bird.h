#ifndef __BIRD_H__
#define __BIRD_H__

#include "Entity.h"
#include "Animation.h"
#include "Point.h"
#include "Box2D/Box2D/Box2D.h"
#include "List.h"

struct SDL_Texture;

class Bird : public Entity
{
public:
	// Constructor
	Bird(iPoint position_);

	// Destructor
	~Bird();

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

	// A set of animations
	Animation birdR;
	Animation birdL;
	Animation birdFlyR;
	Animation birdFlyL;
	Animation birdDieR;
	Animation birdDieL;
	Animation birdHurtR;
	Animation birdHurtL;

	//hitbox for body
	int  Hitbox[16] = {
		0, 2,
		0, 24,
		2, 26,
		24, 26,
		26, 24,
		26, 2,
		24, 0,
		2, 0
	};

	// XML files
	SString catSprites;

	bool birdDead = false;

	// Sound effects indices
	uint birdSound;	
	bool dontPlayAudioBird = false;
	//uint loopFx = 0;
};

#endif // __BIRD_H__