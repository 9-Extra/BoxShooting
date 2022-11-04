#include "StorySystem.h"
#include "Utils.h"

static void gen_mouse(World& world, const SystemContext& context, Vector2f pos, Vector2f speed) {
	unsigned int id = world.assign_entity_id();
	world.cpnt_position[id].data = pos;
	world.cpnt_moving[id].data = speed;
	world.cpnt_render[id].data = RenderDesc::Textured_box(RES_TEXTURE::MOUSE_JERRY);
	Vector2f img_size = texture_size_to_map(context.resources, RES_TEXTURE::MOUSE_JERRY);
	world.cpnt_collision[id].data = CollisionDesc::Box(img_size.x * 0.5f, img_size.y * 0.5f, 1, 0);
	world.entites[id].components = CpntPosition::mask() | CpntMoving::mask() | CpntRender::mask() | CpntCollision::mask();
}

static void gen_duck(World& world, const SystemContext& context, Vector2f pos, Vector2f speed) {
	unsigned int id = world.assign_entity_id();
	world.cpnt_position[id].data = pos;
	world.cpnt_moving[id].data = speed;
	world.cpnt_render[id].data = RenderDesc::Textured_box(RES_TEXTURE::DUCK);
	Vector2f img_size = texture_size_to_map(context.resources, RES_TEXTURE::DUCK);
	world.cpnt_collision[id].data = CollisionDesc::Box(img_size.x * 0.5f, img_size.y * 0.5f, 1, 0);
	world.entites[id].components = CpntPosition::mask() | CpntMoving::mask() | CpntRender::mask() | CpntCollision::mask();
}

static void duck_avoid_cat(World& world, const SystemContext& context) {
	Vector2f cat_pos = world.cpnt_position[ENTITY_ID_PLAYER].data;

	for (unsigned int i = 0; i < ENTITY_MAX_COUNT; i++) {
		if (world.cpnt_render[i].data.textured_box.texture_id == RES_TEXTURE::DUCK) {
			Vector2f p = world.cpnt_position[i].data;
			if ((cat_pos - p).length() < 2.0f) {
				world.cpnt_moving[i].data = (p - cat_pos).normalized() * world.cpnt_moving[i].data.length();
			}
		}
	}
}

void sys_enemy_generate(World& world, const SystemContext& context) {
	std::uniform_real_distribution<float> angle_sampler(-3.14f, 3.14f);
	std::uniform_real_distribution<float> speed_sampler(0.5f, 2.0f);
	std::uniform_real_distribution<float> x_sampler(0.0f, 10.0f);
	std::uniform_real_distribution<float> y_sampler(0.0f, 10.0f);
	std::default_random_engine& e = world.state.random_engine;

	world.state.mouse_generate_cooldown -= context.dt;
	world.state.duck_generate_cooldown -= context.dt;
	if (world.state.mouse_generate_cooldown < 0.0f) {
		Vector2f pos = Vector2f(x_sampler(e), x_sampler(e));
		Vector2f speed = Vector2f(1.0, 1.0).rotate(angle_sampler(e)).normalized() * 0.002f * speed_sampler(e);
		gen_mouse(world, context, pos, speed);

		world.state.mouse_generate_cooldown = 300.0f;
	}

	if (world.state.duck_generate_cooldown < 0.0f) {
		Vector2f pos = Vector2f(x_sampler(e), x_sampler(e));
		Vector2f speed = Vector2f(1.0, 1.0).rotate(angle_sampler(e)).normalized() * 0.003f * speed_sampler(e);
		gen_duck(world, context, pos, speed);

		world.state.duck_generate_cooldown = 600.0f;
	}

	duck_avoid_cat(world, context);


}