#pragma once
#include "GameObject.h"
#include "SystemContext.h"

class GoBullet final : public GameObject<CpntPosition, CpntMoving, CpntRender, CpntCollision> {
public:
	using GameObject::GameObject;

	void create(const SystemContext& context) {
		const float bullet_speed = 0.03f;

		Vector2f cat_pos = world.cpnt_position[ENTITY_ID_PLAYER].data;
		Vector2f speed = (context.devices.input_handler.get_mouse_position() - cat_pos).normalized() * bullet_speed;

		component_data<CpntPosition>() = cat_pos;
		component_data<CpntMoving>() = speed;
		component_data<CpntRender>() = RenderDesc::TexturedBox(RES_TEXTURE::BULLET);
		component_data<CpntCollision>() = CollisionDesc::Box(30 * MAP_RATIO / 2, 30 * MAP_RATIO / 2, 0, 1);

		GameObject::create();
	}


};
