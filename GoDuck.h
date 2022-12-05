#pragma once
#include "GameObject.h"
#include "GoCat.h"

class GoDuck final : public GameObject<CpntPosition, CpntMoving, CpntRender, CpntCollision, CpntAI> {
public:
	using GameObject::GameObject;

	void create(const SystemContext& context, Vector2f pos, Vector2f speed) {
		component_data<CpntPosition>() = pos;
		component_data<CpntMoving>() = speed;
		component_data<CpntRender>() = RenderDesc::TexturedBox(RES_TEXTURE::DUCK);
		Vector2f img_size = texture_size_to_map(context.resources, RES_TEXTURE::DUCK);
		component_data<CpntCollision>() = CollisionDesc::Box(img_size.x * 0.5f, img_size.y * 0.5f, 1, 0);
		component_data<CpntAI>() = duck_ai;

		GameObject::create();
	}

	static void duck_ai(World& world, unsigned int id) {
		Vector2f cat_pos = GoCat(world).component_data<CpntPosition>();
		GoDuck duck(world, id);

		Vector2f p = duck.component_data<CpntPosition>();
		Vector2f& speed = duck.component_data<CpntMoving>();
		if ((cat_pos - p).length() < 2.0f) {
			speed = (p - cat_pos).normalized() * speed.length();
		}
	}
};