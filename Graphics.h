#pragma once

#include <vector>
#include "CGmath.h"
#include "GameDesc.h"
#include <algorithm>
#include "Error.h"

struct Texture {
	std::vector<Color> pixels;
	unsigned int width;
	unsigned int height;
};

class Graphcis {
private:
	friend class Application;

	std::vector<Color> panel;
public:

	Graphcis():
		panel(WINDOW_WIDTH * WINDOW_HEIGHT)
	{}

	void clear(Color color);
	void draw_point(Vector2f p, Color color);
	void draw_box(Vector2f p, int width, int height, Color color);
	void draw_texture_center(Vector2f p, const Texture& texture);
	void draw_texture_upleft(Vector2f p, const Texture& texture);

};
