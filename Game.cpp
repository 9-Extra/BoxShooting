#include "Game.h"

#include "CooldownSystem.h"
#include "RenderSystem.h"
#include "PlayerOperation.h"
#include "MovingSystem.h"
#include "CollisionSystem.h"
#include "StorySystem.h"

void Game::tick(float dt) {
	context.dt = dt;

	//debug_log("Entity count: %u\n", world.entity_count());

	sys_enemy_generate(world, context);
	sys_cooldown(world, context);
	sys_player_operation(world, context);
	sys_moving(world, context);
	sys_collision(world, context);

	sys_render_the_world(world, context);

	world.really_destory_entities();
}