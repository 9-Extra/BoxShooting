#pragma once

#include <vector>

class Element abstract {
protected:
	std::vector<Element> children;

	virtual void tick(float dt) {
		for (Element& e : children) {
			e.tick(dt);
		}
	}
};