#include "Game.h"

#include "CooldownSystem.h"
#include "RenderSystem.h"
#include "PlayerOperation.h"

void Game::tick(float dt) {
	context.dt = dt;

	sys_cooldown(world, context);
	sys_player_operation(world, context);
	sys_render_the_world(world, context);

	world.really_destory_entities();
}