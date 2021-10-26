#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Point.h"
#include "Animation.h"
#include "Box2D/Box2D/Box2D.h"

//implement libraries
struct SDL_Texture;


class Player : public Module
{
public:
	// constructor
	Player(bool startenabled);

	// destructor
	~Player();

	bool Awake(pugi::xml_node&);

	// called when the module is activated
	// loads the necessary textures for the player
	bool Start() override;

	// called at the middle of the application loop
	// processes new input and handles player movement
	bool Update(float dt) override;

	// called at the end of the application loop
	// performs the render call of the player sprite
	bool PostUpdate() override;

	// collision callback, called when the player intersects with another collider
	//implement box2d
/*	void Oncollision(collider* c1, collider* c2) override;   */  

	// save state of the player in the xml file
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	// position of the player in the map
	iPoint position;

	PhysBody* pbody;

	// the player spritesheet loaded into an sdl_texture
	SDL_Texture* texture;
	
	
	// the pointer to the current player animation
	// it will be switched depending on the player's movement direction
	Animation* currentanimation = nullptr;

	// a set of animations
	Animation idleanim;

	Animation jumpanimR;
	Animation jumpanimL;

	Animation weaponanimR;
	Animation weaponanimL;

	Animation leftanim;
	Animation rightanim;
	bool debug;
	// the player's collider
	//box2d
	/*collider* collider = nullptr;*/
};

#endif //!__PLAYER_H__