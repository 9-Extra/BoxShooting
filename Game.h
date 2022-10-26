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

	Game(Devices devices);

	void tick(float dt);

};
