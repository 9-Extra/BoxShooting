#include "Game.h"

#include "CooldownSystem.h"
#include "RenderSystem.h"
#include "PlayerOperation.h"
#include "MovingSystem.h"
#include "CollisionSystem.h"
#include "StorySystem.h"
#include "Utils.h"

Game::Game(const Devices devices, const ResourceManager& resources)
	:context({ devices, resources, 0.0 })
{
	unsigned int id = ENTITY_ID_PLAYER;
	world.cpnt_position[id].data = { 0.5, 0.5 };
	world.cpnt_render[id].data = RenderDesc::Textured_box(RES_TEXTURE::CAT_TOM);
	Vector2f img_size = texture_size_to_map(context.resources, RES_TEXTURE::CAT_TOM);
	world.cpnt_collision[id].data = CollisionDesc::Box(img_size.x * 0.5f, img_size.y * 0.5f, 0, 1);
	world.cpnt_cooldown[id].data = 0.0;
	world.entites[id].components = CpntPosition::mask() | CpntPlayer::mask() | CpntCooldown::mask() | CpntRender::mask() | CpntCollision::mask();

	id = ENTITY_ID_CURSOR;
	world.cpnt_render[id].data = RenderDesc::Textured_box(RES_TEXTURE::CURSOR);
	world.entites[id].components = CpntRender::mask();
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
	sys_render_cursor(world, context);

	world.really_destory_entities();
}