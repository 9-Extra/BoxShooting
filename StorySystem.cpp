#include "StorySystem.h"
#include "Utils.h"

#include "GoDuck.h"
#include "GoMouse.h"

void sys_enemy_generate(World& world, const SystemContext& context) {
	std::uniform_real_distribution<float> angle_sampler(-3.14f, 3.14f);
	std::uniform_real_distribution<float> speed_sampler(0.5f, 2.0f);
	std::uniform_real_distribution<float> x_sampler(0.0f, WINDOW_WIDTH * MAP_RATIO);
	std::uniform_real_distribution<float> y_sampler(0.0f, WINDOW_HEIGHT * MAP_RATIO);
	std::default_random_engine& e = world.state.random_engine;

	world.state.mouse_generate_cooldown -= context.dt;
	world.state.duck_generate_cooldown -= context.dt;
	if (world.state.mouse_generate_cooldown < 0.0f) {
		Vector2f pos = Vector2f(x_sampler(e), x_sampler(e));
		Vector2f speed = Vector2f(1.0, 1.0).rotate(angle_sampler(e)).normalized() * 0.002f * speed_sampler(e);
		GoMouse mouse(world, world.assign_entity_id());
		mouse.create(context, pos, speed);

		world.state.mouse_generate_cooldown = 300.0f;
	}

	if (world.state.duck_generate_cooldown < 0.0f) {
		Vector2f pos = Vector2f(x_sampler(e), x_sampler(e));
		Vector2f speed = Vector2f(1.0, 1.0).rotate(angle_sampler(e)).normalized() * 0.003f * speed_sampler(e);
		GoDuck duck(world, world.assign_entity_id());
		duck.create(context, pos, speed);

		world.state.duck_generate_cooldown = 600.0f;
	}

}