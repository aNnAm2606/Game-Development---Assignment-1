#pragma once
#include "Module.h"

#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -9.81f

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

enum collisionType {
	NONE = -1,
	PLAYER,
	DOG,
	WALL,
	COINS,
	LADDER,
	STONEWIN,
	CHEST,
	STONE,
	CONTROLS,
	TUORIALS
};

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody();
	~PhysBody();

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
	collisionType colType;
};

// Module --------------------------------------
class Physics : public Module, public b2ContactListener // TODO
{
public:
	
	// Constructors & Destructors
	Physics(bool startEnabled);
	~Physics();

	// Main module steps
	bool Start();
	bool PreUpdate();
	bool PostUpdate();
	bool CleanUp();

	// Create basic physics objects
	PhysBody* CreateCircle(int x, int y, int radius, int dynamic);
	PhysBody* CreateRectangle(int x, int y, int width, int height, int dynamic);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, int dynamic);
	PhysBody* CreateCircleSensor(int x, int y, int radius, int dynamic);
	PhysBody* CreateChain(int x, int y, int* points, int size, int dynamic);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

	// Box2D World
	b2World* world;

private:

	// Debug mode
	bool debug;

	// Main ground
	b2Body* ground;

	// Mouse joint
	b2MouseJoint* mouse_joint;
	b2Body* mouse_body;
};