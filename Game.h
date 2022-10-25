#pragma once

#include "InputHandler.h"
#include "Component.h"
#include "World.h"
#include <vector>
#include "SystemContext.h"

/*
* 负责游戏相关的调用
* 游戏数据和程序的结合体，就是游戏的全部
* 游戏数据为world，游戏的逻辑为system
* 游戏的运行还需要依赖一些底层的资源，这些资源并非game所有，只能以引用形式存放在game中
*/
class Game {
private:
	World world;
	Devices devices;
	SystemContext context;

public:

	Game(Devices devices)
		:devices(devices),context({devices, 0.0})
	{
		unsigned int id = world.assign_entity_id();
		world.cpnt_position[id].data = {0.5, 0.5};
		world.cpnt_render[id].data = RenderDesc::Box(30, 30, Color(255, 255, 255));
		world.cpnt_cooldown[id].data = 0.0;
		world.entites[id].components = CpntPosition::mask() | CpntPlayer::mask() | CpntCooldown::mask() | CpntRender::mask();
	};

	void tick(float dt);

};
