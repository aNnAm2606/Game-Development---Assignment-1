#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

struct SDL_Texture;

class Level1 : public Module
{
public:

	Level1(bool startEnabled);

	// Destructor
	virtual ~Level1();

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

#endif // __SCENE_H__