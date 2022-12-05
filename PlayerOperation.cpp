#include "PlayerOperation.h"
#include "GoBullet.h"

static const ComponentBitMask PLAYER_MASK = CpntPosition::mask();

void sys_player_operation(World& world, const SystemContext& context) {
	const InputHandler& sys_input = context.devices.input_handler;
	float dt = context.dt;

	unsigned int i = ENTITY_ID_PLAYER;

	if (mask_contain(world.entites[i].components, PLAYER_MASK)) {
		//debug_log("Time past: %f ms\n", dt);
		Vector2f& p = world.cpnt_position[i].data;
		const float speed = 0.005f;
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
		p.x = std::min<float>(WINDOW_WIDTH * MAP_RATIO, p.x);
		p.y = std::max<float>(0.0f, p.y);
		p.y = std::min<float>(WINDOW_HEIGHT * MAP_RATIO, p.y);

		//debug_log("Box pos: (%f, %f)\n", p.x, p.y);
		static float shooting_delay = 200.0f;
		float& shooting_cooldown = world.cpnt_cooldown[i].data;
		if (sys_input.is_left_button_down() && shooting_cooldown < 0.0f) {
			if (shooting_delay < 0.0f) {
				GoBullet bullet(world, world.assign_entity_id());
				bullet.create(context);
				context.devices.sound_device.play_once(context.resources.sounds[0]);

				shooting_cooldown = 100.0;
			}
			else {
				shooting_delay -= dt;
			}
		}
		else {
			shooting_delay = 200.0f;
		}
	}
}