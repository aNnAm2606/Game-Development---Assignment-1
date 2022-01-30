#ifndef __DOG_H__
#define __DOG_H__

#include "Entity.h"
#include "Animation.h"
#include "Point.h"
#include "Box2D/Box2D/Box2D.h"
#include "List.h"

struct SDL_Texture;

class Dog : public Entity
{
public:
	// Constructor
	Dog(iPoint position_);

	// Destructor
	~Dog();

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
	Animation dogsR;
	Animation dogsL;
	Animation dogsRunR;
	Animation dogsRunL;
	Animation dogsDieR;
	Animation dogsDieL;
	Animation dogsHurtR;
	Animation dogsHurtL;
	Animation dogsBarkR;
	Animation dogsBarkL;

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
	SString dogSprites;

	bool dogDead = false;

	// Sound effects indices
	uint dogSound;
	bool dontPlayAudioDog = false;
};

#endif // __DOG_H__