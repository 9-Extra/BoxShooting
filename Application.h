#pragma once
#include "Display.h"

#include "CGmath.h"

class Application {
private:
	Display display;
	std::vector<Color> panel;
public:
	Application()
		:panel(WINDOW_WIDTH * WINDOW_HEIGHT, Color(255,0,0)), display(WINDOW_WIDTH, WINDOW_HEIGHT)
	{

	}

	int run() {
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



		display.swap(panel.data());
	}
};
