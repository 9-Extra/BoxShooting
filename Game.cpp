#include "Game.h"

#include "CooldownSystem.h"
#include "RenderSystem.h"
#include "PlayerOperation.h"
#include "MovingSystem.h"
#include "CollisionSystem.h"
#include "StorySystem.h"

Game::Game(Devices devices)
	:devices(devices), context({ devices, 0.0 })
{
	unsigned int id = ENTITY_ID_PLAYER;
	world.cpnt_position[id].data = { 0.5, 0.5 };
	world.cpnt_render[id].data = RenderDesc::Box(30, 30, Color(255, 255, 255));
	world.cpnt_cooldown[id].data = 0.0;
	world.entites[id].components = CpntPosition::mask() | CpntPlayer::mask() | CpntCooldown::mask() | CpntRender::mask();
};

void Game::tick(float dt) {
	context.dt = dt;

	//debug_log("Entity count: %u\n", world.dynamic_entity_count());

	sys_enemy_generate(world, context);
	sys_cooldown(world, context);
	sys_player_operation(world, context);
	sys_moving(world, context);
	sys_collision(world, context);

	sys_render_the_world(world, context);

	world.really_destory_entities();
}