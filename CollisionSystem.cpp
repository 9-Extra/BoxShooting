#include "CollisionSystem.h"

static const ComponentBitMask COLLISION_MASK = CpntCollision::mask() | CpntPosition::mask();

void sys_collision(World& world, const SystemContext& context) {
	for (unsigned int i = 0; i < ENTITY_MAX_COUNT; i++) {
		if (mask_contain(world.entites[i].components, COLLISION_MASK)) {
			CollisionDesc c_a = world.cpnt_collision[i].data;
			Vector2f p_a = world.cpnt_position[i].data;
			for (unsigned int j = i + 1; j < ENTITY_MAX_COUNT; j++) {
				if (mask_contain(world.entites[j].components, COLLISION_MASK))
				{
					CollisionDesc c_b = world.cpnt_collision[j].data;
					Vector2f p_b = world.cpnt_position[j].data;
					if (c_a.collision_mask & c_b.group_mask || c_b.collision_mask & c_a.group_mask) {
						Vector2f p = world.cpnt_position[i].data;
						if (c_a.type == CollsionType::BOX && c_b.type == CollsionType::BOX) {
							if (
								std::abs(p_a.x - p_b.x) < (c_a.box_size.half_width + c_b.box_size.half_width) &&
								std::abs(p_a.y - p_b.y) < (c_a.box_size.half_height + c_b.box_size.half_height)
								) 
							{
								unsigned int a, b;
								if (c_a.collision_mask & c_b.group_mask) {
									a = i; b = j;
								}
								else {
									a = j; b = i;
								}
								//保证是a对b进行碰撞
								if (a != ENTITY_ID_PLAYER) {
									world.destroy_entity(a);
								}
								world.destroy_entity(b);
								context.devices.sound_device.play_once(context.resources.sounds[1]);
							}
						}
						else {
							debug_log("Unsupport collision type.\n");
						}
					}
				}
			}
		}
	}
}