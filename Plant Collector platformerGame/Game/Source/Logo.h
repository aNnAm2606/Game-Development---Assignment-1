#ifndef __LOGO_H__
#define __LOGO_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"

struct SDL_Texture;

class Logo : public Module
{
public:
	// Constructor
	Logo(bool startEnabled);

	// Destructor
	~Logo();

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
	SDL_Texture* background;
	SDL_Texture* space;
	SDL_Texture* icon;

	SString screenImage;
	SString StartButton;
	SString logoIcon;

	int frames;
};

#endif	// __LOGO_H__