#pragma once

#include "Component.h"
#include "GameDesc.h"
#include <vector>
#include <algorithm>
#include <random>
#include "Error.h"



struct Entity {
	ComponentBitMask components;
};

struct WorldGlobalState {
	std::default_random_engine random_engine;
	float mouse_generate_cooldown = 1000.0f;
	float duck_generate_cooldown = 1000.0f;

	unsigned int player_scord = 0;

};

//记录所有的状态，只保存数据
class World {
private:
	friend class Game;
	unsigned int valid_id_top;//有效的id的个数，同时指向最后一个有效id的下一个位置
	unsigned int valid_id[ENTITY_MAX_COUNT - ENTITY_CONSTANT_COUNT];

	void really_destory_entities() {
		for (unsigned int i = 0; i < ENTITY_MAX_COUNT; i++) {
			if (entites[i].components & CpntDestroying::mask()) {
				entites[i].components = EMPTY_MASK;
				valid_id[valid_id_top++] = i;
			}
		}
	}

public:
	Entity entites[ENTITY_MAX_COUNT];
	WorldGlobalState state;

	CpntPosition cpnt_position[ENTITY_MAX_COUNT];
	CpntRender cpnt_render[ENTITY_MAX_COUNT];
	CpntCooldown cpnt_cooldown[ENTITY_MAX_COUNT];
	CpntMoving cpnt_moving[ENTITY_MAX_COUNT];
	CpntCollision cpnt_collision[ENTITY_MAX_COUNT];
	CpntAI cpnt_ai[ENTITY_MAX_COUNT];

	World();

	unsigned int dynamic_entity_count() const{
		return ENTITY_MAX_COUNT - ENTITY_CONSTANT_COUNT - valid_id_top;
	}


	unsigned int assign_entity_id() {
		if (valid_id_top == 0) {
			GameError(L"实体数量超过上限");
		}
		--valid_id_top;
		return valid_id[valid_id_top];
	}
	
	//仅仅进行记录，真正的回收交给game
	void destroy_entity(unsigned int id) {
		assert(entites[id].components != EMPTY_MASK);
		entites[id].components |= CpntDestroying::mask();
	}
};
