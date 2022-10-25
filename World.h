#pragma once

#include "InputSystem.h"
#include "Entity.h"

class EntRoot : public Entity {
	friend class World;
};

struct Box {
	Vector2f p;
	Color color;
	int w, h;
};

struct Bullet {
	Vector2f p;
	Vector2f speed;
};

class World {
private:
	InputSystem sys_input;

public:
	Box box{ {0.5, 0.5}, Color{255, 255, 255}, 20, 20 };
	std::vector<Bullet> bullets;
	float shooting_cooldown = 0.0f;

	World() 
	{
	};

	void tick(float dt);

};
