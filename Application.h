#pragma once
#include "ImageLoader.h"
#include "SoundLoader.h"
#include "ModelLoader.h"
#include "Display.h"

#include "CGmath.h"
#include "Game.h"

/*
* 代表整个程序的逻辑
* 持有底层资源，将资源的引用传给上层以供使用
*/
class Application {
	using time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;
private:
	Graphcis graphics;
	SoundDevice sound_device;

	ResourceManager resources;

	std::unique_ptr<Game> game;

	time_point last;

	float reset_timer() {
		time_point now = std::chrono::high_resolution_clock::now();
		float past_time = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>((now - last)).count();
		last = now;

		return past_time;
	}

	void load_resources();

public:
	Application();

	int run();
	void doFrame();
	
};
