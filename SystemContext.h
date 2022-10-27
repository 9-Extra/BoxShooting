#pragma once
/*
* 保存所有system运行时需要的非world信息
*/

#include "World.h"
#include "Devices.h"
#include "ResourceManager.h"

struct SystemContext {
	const Devices devices;
	const ResourceManager& resources;
	float dt;
};

