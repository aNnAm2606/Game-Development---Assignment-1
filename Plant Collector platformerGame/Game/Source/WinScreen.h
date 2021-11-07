#ifndef __WIN_SCREEN_H__
#define __WIN_SCREEN_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"

struct SDL_Texture;

class WinScreen : public Module
{
public:
	// Constructor
	WinScreen(bool startEnabled);

	// Destructor
	~WinScreen();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(float dt) override;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate() override;

	bool CleanUp();
public:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* bg1;
	SDL_Texture* title;
	SDL_Texture* button;
	SDL_Texture* character;

	SString WinTitle;
	SString background;
	SString characterFile;
	SString restartButton;

	Animation idle;
	Animation* currentAnimation = nullptr;

	int frames;
};

#endif	// __SCENEINTRO_H__