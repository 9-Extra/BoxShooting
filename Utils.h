#pragma once

#include "SystemContext.h"

inline Vector2f texture_size_to_map(const ResourceManager& mng, RES_TEXTURE id) {
	const Texture& t = mng.get_texture(id);
	return Vector2f(t.width * MAP_RATIO, t.height * MAP_RATIO);
}