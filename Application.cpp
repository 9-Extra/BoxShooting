#include "Application.h"

void Application::load_resources() {
	debug_log("Start loading");
	SoundLoader loader(resources);
	loader.load_sound(L"resources\\sound\\shooting.wav", RES_SOUND::SHOOTING);
	loader.load_sound(L"resources\\sound\\enemy_dead.wav", RES_SOUND::DEAD_AAAAA);

	ImageLoader image_loader(resources);
	image_loader.load_image(L"resources\\texture\\cursor.png", RES_TEXTURE::CURSOR);
	image_loader.load_image(L"resources\\texture\\tom.bmp", RES_TEXTURE::CAT_TOM);
	image_loader.load_image(L"resources\\texture\\duck.jpg", RES_TEXTURE::DUCK);
	image_loader.load_image(L"resources\\texture\\jerry.bmp", RES_TEXTURE::MOUSE_JERRY);
	image_loader.load_image(L"resources\\texture\\dog.bmp", RES_TEXTURE::DOG);
	image_loader.load_image(L"resources\\texture\\bullet.png", RES_TEXTURE::BULLET);
	debug_log("Loading compelete");
}

Application::Application()
	:graphics(),
	display(WINDOW_WIDTH, WINDOW_HEIGHT)
{
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_UNAWARE);

	load_resources();

	game = std::make_unique<Game>(Devices{ graphics, sound_device,input_handler }, resources);
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

	graphics.clear(Color(255, 255, 255));

	game->tick(reset_timer());

	display.swap(graphics.panel.data());
}