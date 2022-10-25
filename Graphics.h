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

	void clear(Color color) {
		std::fill(panel.begin(), panel.end(), color);
	}

	void draw_box(float x, float y, int width, int height, Color color) {
		//debug_log("Drawing box at (%f, %f)\n", x, y);
		int center_x = (int)(x * WINDOW_WIDTH);
		int center_y = (int)(y * WINDOW_HEIGHT);
		int x_start = std::max<int>(0, center_x - width / 2);
		int y_start = std::max<int>(0, center_y - height / 2);
		int x_end = std::min<int>(WINDOW_WIDTH - 1, center_x + width / 2);
		int y_end = std::min<int>(WINDOW_HEIGHT - 1, center_y + height / 2);
		for (int y = y_start; y < y_end; y++) {
			for (int x = x_start; x < x_end; x++) {
				panel[y * WINDOW_WIDTH + x] = color;
			}
		}

	}

};
