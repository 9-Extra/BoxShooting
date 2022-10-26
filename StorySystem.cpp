#include "StorySystem.h"

void sys_enemy_generate(World& world, const SystemContext& context) {
	std::uniform_real_distribution<float> angle_sampler(-0.2f, 0.2f);
	std::uniform_real_distribution<float> speed_sampler(0.5f, 2.0f);

	world.state.enemy_generate_cooldown -= context.dt;
	if (world.state.enemy_generate_cooldown < 0.0f) {
		unsigned int id = world.assign_entity_id();
		world.cpnt_position[id].data = Vector2f(0.0, 0.0);
		float r = angle_sampler(world.state.random_engine);
		float s = speed_sampler(world.state.random_engine);
		world.cpnt_moving[id].data = Vector2f(1.0, 1.0).rotate(r).normalized() * 0.01f * s;
		world.cpnt_render[id].data = RenderDesc::Box(40, 40, { 0, 0, 255 });
		world.cpnt_collision[id].data = CollisionDesc::Box(40 * MAP_RATIO / 2, 40 * MAP_RATIO / 2, 1, 0);
		world.entites[id].components = CpntPosition::mask() | CpntMoving::mask() | CpntRender::mask() | CpntCollision::mask();

		world.state.enemy_generate_cooldown = 400.0f;
	}




}