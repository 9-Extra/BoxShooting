#pragma once

#include "Component.h"
#include "GameDesc.h"
#include <vector>
#include <algorithm>
#include "Error.h"

struct Entity {
	ComponentBitMask components;
};

//记录所有的状态，只保存数据
class World {
private:
	friend class Game;
	std::vector<unsigned int> valid_id;

	void really_destory_entities() {
		for (unsigned int i = 0; i < ENTITY_MAX; i++) {
			if (entites[i].components & CpntDestroying::mask()) {
				entites[i].components = EMPTY_MASK;
				valid_id.push_back(i);
			}
		}
	}

public:
	Entity entites[ENTITY_MAX];

	CpntPosition cpnt_position[ENTITY_MAX];
	CpntRender cpnt_render[ENTITY_MAX];
	CpntCooldown cpnt_cooldown[ENTITY_MAX];
	CpntMoving cpnt_moving[ENTITY_MAX];
	CpntCollision cpnt_collision[ENTITY_MAX];

	World()
	{
		std::fill(entites, entites + ENTITY_MAX, Entity{EMPTY_MASK});
		valid_id.resize(ENTITY_MAX);

		for (unsigned int i = 0; i < ENTITY_MAX; i++)
		{
			valid_id[i] = i;
		}
	}

	unsigned int entity_count() const{
		return ENTITY_MAX - (unsigned int)valid_id.size();
	}


	unsigned int assign_entity_id() {
		if (valid_id.empty()) {
			GameError(L"实体数量超过上限");
		}
		unsigned int id = valid_id.back();
		valid_id.pop_back();
		return id;
	}
	
	//仅仅进行记录，真正的回收交给game
	void destroy_entity(unsigned int id) {
		assert(entites[id].components != EMPTY_MASK);
		entites[id].components |= CpntDestroying::mask();
	}
};
