#include "Application.h"

void Application::load_resources() {
	// The factory pointer
	ImageLoader image_loader;

	image_loader.load_image(L"resources\\texture\\cursor.png", resources.textures[0]);

	sound_device.load_sound(L"resources\\sound\\finished.wav", resources.sounds[0]);
}

Application::Application()
	:graphics(),
	display(WINDOW_WIDTH, WINDOW_HEIGHT)
{
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_UNAWARE);

	load_resources();

	game = std::make_unique<Game>(Devices{ graphics, input_handler }, resources);
}

int Application::run() {
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

void Application::doFrame() {

	graphics.clear(Color(0, 0, 0));

	game->tick(reset_timer());

	display.swap(graphics.panel.data());
}