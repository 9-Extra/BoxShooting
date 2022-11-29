#pragma once
#include "GameObject.h"

class GoMouse final : public GameObject<CpntPosition, CpntMoving, CpntRender, CpntCollision> {
public:
	using GameObject::GameObject;

	void create(const SystemContext& context, Vector2f pos, Vector2f speed) {
		component_data<CpntPosition>() = pos;
		component_data<CpntMoving>() = speed;
		component_data<CpntRender>() = RenderDesc::Textured_box(RES_TEXTURE::MOUSE_JERRY);
		Vector2f img_size = texture_size_to_map(context.resources, RES_TEXTURE::MOUSE_JERRY);
		component_data<CpntCollision>() = CollisionDesc::Box(img_size.x * 0.5f, img_size.y * 0.5f, 1, 0);

		GameObject::create();
	}


};