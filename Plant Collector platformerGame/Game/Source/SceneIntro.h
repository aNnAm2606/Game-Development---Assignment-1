#ifndef __SCENEINTRO_H__
#define __SCENEINTRO_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class SceneIntro : public Module
{
public:
	// Constructor
	SceneIntro(bool startEnabled);

	// Destructor
	~SceneIntro();

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
	SDL_Texture* bg2;
	SDL_Texture* bg3;
	SDL_Texture* bg4;
	SDL_Texture* bg5;
};

#endif	// __SCENEINTRO_H__