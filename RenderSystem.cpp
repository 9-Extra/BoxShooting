#include "RenderSystem.h"

static const ComponentBitMask RENDER_MASK = CpntRender::mask() | CpntPosition::mask();

static inline void render_entity(Vector2f p, RenderDesc& rd, Graphcis& g, const ResourceManager& mng) {
	switch (rd.type) {
	case RenderType::BOX: {
		g.draw_box(p, rd.box.width, rd.box.height, rd.box.color);
		break;
	}
	case RenderType::TEXTURED_BOX: {
		g.draw_texture_center(p, mng.get_texture(rd.textured_box.texture_id));
		break;
	}
	case RenderType::MODEL: {
		mng.models[rd.model.model_id].draw(g.p_command_list.get());
		break;
	}
	default: {
		GameError(L"Unsupport RenderType");
	}
	}
}

void sys_render_the_world(World & world, const SystemContext & context) {
	context.devices.grahics.render_panel();
	context.devices.grahics.model_render_context();

	for (unsigned int i = 0; i < ENTITY_MAX_COUNT; i++) {
		if (mask_contain(world.entites[i].components, RENDER_MASK)) {
			Vector2f p = world.cpnt_position[i].data;
			RenderDesc& rd = world.cpnt_render[i].data;
			render_entity(p, rd, context.devices.grahics, context.resources);
		}
	}

	context.devices.grahics.model_render_end();
}
void sys_render_cursor(World& world, const SystemContext& context) {
	unsigned int id = ENTITY_ID_CURSOR;
	if (mask_contain(world.entites[id].components, CpntRender::mask())) {
		Vector2f p = context.devices.input_handler.get_mouse_position();
		render_entity(p, world.cpnt_render[id].data, context.devices.grahics, context.resources);
	}
}
