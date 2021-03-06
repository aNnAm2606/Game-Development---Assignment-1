#ifndef __LEVEL_2_H__
#define __LEVEL_2_H__

#include "Module.h"

struct SDL_Texture;

class Level2 : public Module
{
public:

	Level2(bool startEnabled);

	// Destructor
	virtual ~Level2();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:
	SDL_Texture* img;
	SDL_Texture* sky;
	SDL_Texture* forestBushes;
	SDL_Texture* treesBack;
	SDL_Texture* treesMid;
	SDL_Texture* treesMidClose;
	SDL_Texture* particles;
	SDL_Texture* treesClose;
	SDL_Texture* particles1;
	SDL_Texture* bushes;
	SDL_Texture* mist;

public:
	bool F6Load = false;
};

#endif // __LEVEL_2_H__