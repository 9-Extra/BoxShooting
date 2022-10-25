#include "PlayerOperation.h"

static const ComponentBitMask PLAYER_MASK = CpntPosition::mask() | CpntPlayer::mask();

void sys_player_operation(World& world, const SystemContext& context) {
	const InputHandler& sys_input = context.devices.input_handler;
	float dt = context.dt;

	for (unsigned int i = 0; i < ENTITY_MAX; i++) {
		if (world.entites[i].components & PLAYER_MASK) {
			//debug_log("Time past: %f ms\n", dt);
			Vector2f& p = world.cpnt_position[i].data;
			const float speed = 0.001f;
			//box.p = sys_input.get_mouse_position();
			if (sys_input.is_keydown('W')) {
				p.y -= speed * dt;
			}
			if (sys_input.is_keydown('S')) {
				p.y += speed * dt;
			}
			if (sys_input.is_keydown('A')) {
				p.x -= speed * dt;
			}
			if (sys_input.is_keydown('D')) {
				p.x += speed * dt;
			}

			p.x = std::max<float>(0.0f, p.x);
			p.x = std::min<float>(1.0f, p.x);
			p.y = std::max<float>(0.0f, p.y);
			p.y = std::min<float>(1.0f, p.y);

			//debug_log("Box pos: (%f, %f)\n", p.x, p.y);

			/*
			const float bullet_speed = 0.01f;

			for (Bullet& b : bullets) {
				b.p = b.p + b.speed;
			}

			unsigned int ind = 0;
			for (unsigned int i = 0; i < bullets.size(); i++) {
				Vector2f p = bullets[i].p;
				if (p.x > 1.0f || p.x < 0.0f || p.y < 0.0f || p.y >= 1.0f) {
					continue;
				}
				bullets[ind] = bullets[i];
				ind++;
			}
			bullets.resize(ind);
			*/

			float& shooting_cooldown = world.cpnt_cooldown[i].data;
			if (sys_input.is_left_button_down() && shooting_cooldown < 0.0f) {
				//bullets.emplace_back(Bullet{ box.p, (sys_input.get_mouse_position() - box.p).normalized() * bullet_speed });
				debug_log("Generate a bullet!\n");
				shooting_cooldown = 50.0;
			}
		}
	}
}