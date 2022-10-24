#pragma once

#include <vector>

class Entity abstract {
protected:
	std::vector<Entity> children;

	virtual void tick(float dt) {
		for (Entity& e : children) {
			e.tick(dt);
		}
	}
};