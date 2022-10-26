#include "PlayerOperation.h"

static const ComponentBitMask PLAYER_MASK = CpntPosition::mask() | CpntPlayer::mask();

void sys_player_operation(World& world, const SystemContext& context) {
	const InputHandler& sys_input = context.devices.input_handler;
	float dt = context.dt;

	for (unsigned int i = 0; i < ENTITY_MAX; i++) {
		if (mask_contain(world.entites[i].components, PLAYER_MASK)) {
			//debug_log("Time past: %f ms\n", dt);
			Vector2f& p = world.cpnt_position[i].data;
			const float speed = 0.01f;
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

			
			const float bullet_speed = 0.01f;
		

			float& shooting_cooldown = world.cpnt_cooldown[i].data;
			if (sys_input.is_left_button_down() && shooting_cooldown < 0.0f) {
				//bullets.emplace_back(Bullet{ box.p, (sys_input.get_mouse_position() - box.p).normalized() * bullet_speed });
				unsigned int id = world.assign_entity_id();
				world.cpnt_position[id].data = p;
				Vector2f speed = (sys_input.get_mouse_position() - p).normalized() * bullet_speed;
				world.cpnt_moving[id].data = speed;
				world.cpnt_render[id].data = RenderDesc::Box(5, 5, Color(255,0,0 ));
				
				world.entites[id].components = CpntPosition::mask() | CpntMoving::mask() | CpntRender::mask();
				shooting_cooldown = 50.0;
			}
		}
	}
}