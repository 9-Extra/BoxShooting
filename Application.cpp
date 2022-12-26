#include "Application.h"

void Application::load_resources() {
	debug_log("Start loading\n");
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
	debug_log("Loading compeleted\n");

	ModelLoader model_loader(resources, graphics);
	const std::vector<Vertex> cube_vertices{
		 { { -1.0f, 1.0f, -1.0f}, { 0.0f, 0.0f } },    // Back Top Left
			{ { 1.0f, 1.0f, -1.0f}, { 0.0f, 0.0f } },    // Back Top Right
			{ { 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f } },    // Front Top Right
			{ { -1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} },    // Front Top Left

			{ { -1.0f, -1.0f, -1.0f}, { 0.0f, 0.0f } },    // Back Bottom Left
			{ { 1.0f, -1.0f, -1.0f}, { 0.0f, 0.0f } },    // Back Bottom Right
			{ { 1.0f, -1.0f, 1.0f}, { 0.0f, 0.0f } },    // Front Bottom Right
			{ { -1.0f, -1.0f, 1.0f}, { 0.0f, 0.0f } },    // Front Bottom Left
	};

	const std::vector<unsigned int> cube_indices
	{
		0, 1, 3,
		1, 2, 3,

		3, 2, 7,
		6, 7, 2,

		2, 1, 6,
		5, 6, 1,

		1, 0, 5,
		4, 5, 0,

		0, 3, 4,
		7, 4, 3,

		7, 6, 4,
		5, 4, 6,
	};
	model_loader.model_from_memory(cube_vertices, cube_indices, 0u);


	const std::vector<Vertex> triangle_vertices{
		 { { 0.0f, 0.5f, 1.0f}, { 0.0f, 0.0f } },
			{ { 0.5f, -0.5f, 1.0f}, { 0.0f, 0.0f } },
			{ { -0.5f, -0.5f, 1.0f}, {0.0f, 0.0f } }
	};

	const std::vector<unsigned int> triangle_indices
	{
		0, 1, 2
	};
	model_loader.model_from_memory(triangle_vertices, triangle_indices, 1u);


}

Application::Application()
	:graphics(WINDOW_WIDTH, WINDOW_HEIGHT)
{
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

	graphics.swap();
}