#include "RenderSystem.h"

static const ComponentBitMask RENDER_MASK = CpntRender::mask() | CpntPosition::mask();

void sys_render_the_world(World& world, const SystemContext& context) {
	for (unsigned int i = 0; i < ENTITY_MAX; i++) {
		if (mask_contain(world.entites[i].components, RENDER_MASK)) {
			Vector2f p = world.cpnt_position[i].data;
			RenderDesc& rd = world.cpnt_render[i].data;
			switch (rd.type) {
			case RenderType::BOX: {
				context.devices.grahics.draw_box(p.x, p.y, rd.box_size.width, rd.box_size.height, rd.color);
				break;
			}
			}

		}
	}
}