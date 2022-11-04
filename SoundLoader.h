#pragma once

#include "ResourceManager.h"

class SoundLoader {
private:
	ResourceManager& mng;

public:
	SoundLoader(ResourceManager& mng)
		:mng(mng)
	{}

	void load_sound(LPCWSTR path, const RES_SOUND sound_id);



};
