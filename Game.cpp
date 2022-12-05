#include "Game.h"

#include "CooldownSystem.h"
#include "RenderSystem.h"
#include "PlayerOperation.h"
#include "MovingSystem.h"
#include "CollisionSystem.h"
#include "StorySystem.h"
#include "AISystem.h"
#include "Utils.h"

#include "GoCat.h"

void Game::render_hub(World& world, const SystemContext& context)
{
	static wchar_t buffer[128];
	wsprintfW(buffer, L"%u", world.state.player_scord);
	context.devices.grahics.draw_text_upleft(5, 5, Color(255, 0, 0), buffer); 
}

Game::Game(const Devices devices, const ResourceManager& resources)
	:context({ devices, resources, 0.0 })
{
	GoCat cat(world);
	cat.create(context);

	unsigned id = ENTITY_ID_CURSOR;
	world.cpnt_render[id].data = RenderDesc::TexturedBox(RES_TEXTURE::CURSOR);
	world.entites[id].components = CpntRender::mask();
};

void Game::tick(float dt) {
	context.dt = dt;

	//debug_log("Entity count: %u\n", world.dynamic_entity_count());

	sys_enemy_generate(world, context);
	sys_ai_run(world, context);
	sys_cooldown(world, context);
	sys_player_operation(world, context);
	sys_moving(world, context);
	sys_collision(world, context);

	sys_render_the_world(world, context);
	sys_render_cursor(world, context);

	render_hub(world, context);

	world.really_destory_entities();
}