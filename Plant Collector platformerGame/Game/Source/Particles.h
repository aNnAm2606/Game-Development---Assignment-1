#ifndef __PARTICLES_H__
#define __PARTICLES_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "Box2D/Box2D/Box2D.h"
#include "List.h"

struct SDL_Texture;

enum Type {
	NOTHING = -1,
	COIN,
	EXPLOSIONS,
	HEART,
	KEY,
	POWERUPS
};

//Creating coins
struct Particle
{
	bool pendingToDelete = false;
	iPoint position;
	Type type;
	PhysBody* body;

	Animation coins;
	Animation hearts;
	Animation keys;

};


class Particles : public Module
{
public:
	// Constructor
	Particles(bool startEnabled);

	// Destructor
	~Particles();

	// Awake
	bool Awake(pugi::xml_node&);

	// Create Particles
	void CreateParticles(Type type, int x, int y);

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

	// Collision callback, called when the player intersects with another collider
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB) override;

	// Save and load the position of the playerç
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	bool CleanUp() override;

private:

	SDL_Texture* coin;
	SString textureCoin;

	SDL_Texture* heart;
	SString textureHeart;

	SDL_Texture* key;
	SString textureKey;

	List<Particle*> particles;
};

#endif // __PARTICLES_H__