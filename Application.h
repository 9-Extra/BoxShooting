#pragma once
#include "Display.h"

#include "CGmath.h"
#include "World.h"
#include <algorithm>

class Application {
private:
	using time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;

	Display display;
	std::vector<Color> panel;
	std::unique_ptr<World> world;
	time_point last;
public:
	Application()
		:panel(WINDOW_WIDTH * WINDOW_HEIGHT, Color(255,0,0)), 
		display(WINDOW_WIDTH, WINDOW_HEIGHT),
		world(std::make_unique<World>())
	{
	}

	int run() {
		last = std::chrono::high_resolution_clock::now();

		MSG msg;
		while (true)
		{
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				DispatchMessage(&msg);
				if (msg.message == WM_QUIT) {
					return (int)msg.wParam;
				}
			}
			doFrame();
		}
	}

	void doFrame() {

		time_point now = std::chrono::high_resolution_clock::now();
		float past_time = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>((now - last)).count();
		last = now;

		world->tick(past_time);

		draw_box(world->box.p.x, world->box.p.y, world->box.w, world->box.h, world->box.color);
		
		debug_log("Bullet count: %zu\n", world->bullets.size());
		for (Bullet& b : world->bullets) {
			//debug_log("%f, %f\n", b.p.x , b.p.y);
			draw_box(b.p.x, b.p.y, 5, 5, Color(255, 0, 0));
		}

		display.swap(panel.data());
		std::fill(panel.begin(), panel.end(), Color(0, 0, 0));
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
