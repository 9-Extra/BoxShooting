#pragma once
/*
* 保存所有system运行时需要的非world信息
*/

#include "ResourceManager.h"
#include "Devices.h"
#include "World.h"

struct SystemContext {
	const Devices devices;
	const ResourceManager& resources;
	float dt;
};

