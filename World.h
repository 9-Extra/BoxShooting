#pragma once

#include "InputSystem.h"
#include "Element.h"

class ElemRoot : public Element{
	friend class World;
};

class World {
private:
	InputSystem sys_input;
	ElemRoot root;

public:
	World() {};

	void tick(float dt) {
		//debug_log("Time past: %f ms\n", dt);
		root.tick(dt);
	}

};
