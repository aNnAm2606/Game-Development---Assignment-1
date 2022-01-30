#ifndef __CAT_H__
#define __CAT_H__

#include "Entity.h"
#include "Animation.h"
#include "Point.h"
#include "Box2D/Box2D/Box2D.h"
#include "List.h"

struct SDL_Texture;

class Cat : public Entity
{
public:
	// Constructor
	Cat(iPoint position_);

	// Destructor
	~Cat();

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
	Animation catsR;
	Animation catsL;
	Animation catsRunR;
	Animation catsRunL;
	Animation catsDieR;
	Animation catsDieL;
	Animation catsHurtR;
	Animation catsHurtL;

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

	bool catDead = false;

	// Sound effects indices
	uint catSound;	
	bool dontPlayAudioCat = false;
	//uint loopFx = 0;
};

#endif // __CAT_H__