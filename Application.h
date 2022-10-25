#pragma once
#include "Display.h"

#include "CGmath.h"
#include "World.h"

class Application {
private:
	using time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;

	Display display;
	std::vector<Color> panel;
	std::unique_ptr<World> world;
	time_point last;

	float reset_timer() {
		time_point now = std::chrono::high_resolution_clock::now();
		float past_time = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>((now - last)).count();
		last = now;

		return past_time;
	}

public:
	Application()
		:panel(WINDOW_WIDTH * WINDOW_HEIGHT, Color(255,0,0)), 
		display(WINDOW_WIDTH, WINDOW_HEIGHT),
		world(std::make_unique<World>())
	{
	}

	int run() {
		reset_timer();

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

		world->tick(reset_timer());

		display.swap(panel.data());
	}
};
