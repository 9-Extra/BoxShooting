#pragma once
#include "Display.h"

#include "CGmath.h"
#include "Game.h"
#include "Graphics.h"

/*
* 代表整个程序的逻辑
* 持有底层资源，将资源的引用传给上层以供使用
*/
class Application {
private:
	using time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;

	Display display;
	Graphcis graphics;
	std::unique_ptr<Game> game;
	time_point last;

	float reset_timer() {
		time_point now = std::chrono::high_resolution_clock::now();
		float past_time = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>((now - last)).count();
		last = now;

		return past_time;
	}

public:
	Application()
		:graphics(),
		display(WINDOW_WIDTH, WINDOW_HEIGHT)
	{
		SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_UNAWARE);
		

		game = std::make_unique<Game>(Devices{ graphics, input_handler }, ResourceManager{});
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

		graphics.clear(Color(0, 0, 0));

		game->tick(reset_timer());

		display.swap(graphics.panel.data());
	}

	
};
