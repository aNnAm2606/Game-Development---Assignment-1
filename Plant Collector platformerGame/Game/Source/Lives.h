#ifndef __LIVES_H__
#define __LIVES_H__

#include "Module.h"
#include "Animation.h"
#include "Box2D/Box2D/Box2D.h"

struct SDL_Texture;



class Lives : public Module
{
public:
	// Constructor
	Lives(bool startEnabled);

	// Destructor
	~Lives();

	// Awake
	bool Awake(pugi::xml_node&);

	// Create Particles

	// Called when the module is activated
	// Loads the necessary textures for the player
	bool Start() override;

	// Called at the start of the application loop
	// Processes new input and handles player movement
	bool PreUpdate();

	// Called at the middle of the application loop
	// Processes new input and handles player movement
	bool Update(float dt) override;

	// Called at the end of the application loop
	// Performs the render call of the player sprite
	bool PostUpdate() override;


	// Save and load the position of the playerç
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	bool CleanUp() override;

private:

	SDL_Texture* hearts3;
	SDL_Texture* hearts2;
	SDL_Texture* hearts1;
	SString textureHeart;

};

#endif // __LIVES_H__