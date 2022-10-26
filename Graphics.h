#pragma once

#include <vector>
#include "CGmath.h"
#include "GameDesc.h"
#include <algorithm>

class Graphcis {
private:
	friend class Application;

	std::vector<Color> panel;
public:

	Graphcis():
		panel(WINDOW_WIDTH * WINDOW_HEIGHT)
	{}

	void clear(Color color);
	void draw_point(float x, float y, Color color);
	void draw_box(float x, float y, int width, int height, Color color);

};
