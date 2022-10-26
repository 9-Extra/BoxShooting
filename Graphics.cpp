#include "Graphics.h"

void Graphcis::clear(Color color) {
	std::fill(panel.begin(), panel.end(), color);
}
void Graphcis::draw_point(float x, float y, Color color) {
	int center_x = (int)(x / MAP_RATIO);
	int center_y = (int)(y / MAP_RATIO);
	if (center_x >= 0 && center_x < WINDOW_WIDTH && center_y >= 0 && center_y < WINDOW_HEIGHT) {
		panel[center_y * WINDOW_WIDTH + center_x] = color;
	}
}
void Graphcis::draw_box(float x, float y, int width, int height, Color color) {
	//debug_log("Drawing box at (%f, %f)\n", x, y);
	int center_x = (int)(x / MAP_RATIO);
	int center_y = (int)(y / MAP_RATIO);
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