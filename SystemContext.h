#pragma once
/*
* 保存所有system运行时需要的非world信息
*/

#include "World.h"
#include "Devices.h"

struct SystemContext {
	Devices devices;
	float dt;
};
