#pragma once
#include "SoundDevice.h"
#include "Graphics.h"
#include "ResourceTable.h"

/*
* 鉴于使用的资源都是固定的，即运行中每个对象要使用的资源不需要动态指定
* 所有的资源在运行前由Application完成加载，由Application管理，放在ResourceManager中
* ResourceManger的引用传递给Game使用
* 每一类的资源放在数组中，其位置固定
*/
#define RES_TEXTURE_MAX 100
#define RES_SOUND_MAX 100

struct ResourceManager {
	Texture textures[RES_TEXTURE_MAX];
	Sound sounds[RES_SOUND_MAX];

	const Texture& get_texture(RES_TEXTURE id) const {
		return textures[static_cast<unsigned int>(id)];
	}

	const Sound& get_sound(RES_SOUND id) const{
		return sounds[static_cast<unsigned int>(id)];
	}
};