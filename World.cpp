#include "World.h"

void World::tick(float dt) {
	//debug_log("Time past: %f ms\n", dt);
	const float speed = 0.001f;
	//box.p = sys_input.get_mouse_position();
	if (sys_input.is_keydown('W')) {
		box.p.y -= speed * dt;
	}
	if (sys_input.is_keydown('S')) {
		box.p.y += speed * dt;
	}
	if (sys_input.is_keydown('A')) {
		box.p.x -= speed * dt;
	}
	if (sys_input.is_keydown('D')) {
		box.p.x += speed * dt;
	}

	box.p.x = std::max<float>(0.0f, box.p.x);
	box.p.x = std::min<float>(1.0f, box.p.x);
	box.p.y = std::max<float>(0.0f, box.p.y);
	box.p.y = std::min<float>(1.0f, box.p.y);

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

	shooting_cooldown -= dt;
	if (sys_input.is_left_button_down() && shooting_cooldown < 0.0f) {
		bullets.emplace_back(Bullet{ box.p, (sys_input.get_mouse_position() - box.p).normalized() * bullet_speed });
		shooting_cooldown = 50.0;
	}
	shooting_cooldown = std::max<float>(0.0f, shooting_cooldown);
}