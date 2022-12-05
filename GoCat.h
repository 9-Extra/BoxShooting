#pragma once
#include "GameObject.h"
#include "SystemContext.h"
#include "Utils.h"

class GoCat final : public GameObject<CpntPosition, CpntCooldown, CpntRender, CpntCollision> {
public:
	GoCat(World& world)
		: GameObject(world, ENTITY_ID_PLAYER)
	{}

	void create(const SystemContext& context) {
		Vector2f img_size = texture_size_to_map(context.resources, RES_TEXTURE::CAT_TOM);

		component_data<CpntPosition>() = ratio_to_map({ 0.5, 0.5 });
		component_data<CpntRender>() = RenderDesc::TexturedBox(RES_TEXTURE::CAT_TOM);
		component_data<CpntCollision>() = CollisionDesc::Box(img_size.x * 0.5f, img_size.y * 0.5f, 0, 1);
		component_data<CpntCooldown>() = 0.0f;
		GameObject::create();
	}

};

