#pragma once

#include "InputSystem.h"
#include "Entity.h"

class EntRoot : public Entity {
	friend class World;
};

class World {
private:
	InputSystem sys_input;
	EntRoot root;

public:
	World() {};

	void tick(float dt) {
		//debug_log("Time past: %f ms\n", dt);
		root.tick(dt);
	}

};
