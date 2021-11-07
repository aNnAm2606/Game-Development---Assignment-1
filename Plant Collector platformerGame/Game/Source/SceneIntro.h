#ifndef __SCENEINTRO_H__
#define __SCENEINTRO_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"

struct SDL_Texture;

class SceneIntro : public Module
{
public:
	// Constructor
	SceneIntro(bool startEnabled);

	// Destructor
	~SceneIntro();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(float dt) override;

	// Load last game
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate() override;

	bool CleanUp();
public:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* bg1;
	SDL_Texture* bg2;
	SDL_Texture* bg3;
	SDL_Texture* character;
	SDL_Texture* bg5;

	SString screenImage;
	SString StartButton;
	SString title;
	SString playerSprites;

	Animation walk;
	Animation* currentAnimation = nullptr;

	iPoint cPos;

	bool level1;
	bool level2;
	int frames;
};

#endif	// __SCENEINTRO_H__