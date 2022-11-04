#pragma once

#include "CGmath.h"
#include "ResourceTable.h"

/*
使用ECS架构
1. 系统进行操作，但不记录状态，被实现为纯粹的函数
2. 组件记录状态，就是一坨数据
3. 实体是组件的集合，系统通过操作组件操作实体

4. 每个系统在被激活时，扫描world中的每一个实体，找到其中与其相关的组件执行操作
5. system在tick时按顺序激活
6. 每个实体在创建时立即完成创建，销毁时在所有的系统完成调用后再真正销毁

7. Component本体以数组的形式保存在World中，在Entity中以bitset的形式记录其Component
8. 系统使用mask以快速确定实体是否需要被其操作

*/
//只保存状态，不存在行为

using ComponentBitMask = unsigned long long int;
const ComponentBitMask EMPTY_MASK = 0;

inline bool mask_contain(ComponentBitMask a, ComponentBitMask b) {
	return (a & b) == b;
}

template <typename T, unsigned int ID>
class Component{
	static_assert(ID < sizeof(ComponentBitMask) * 8, "ID too big!");
	friend class World;
public:
	T data;

	constexpr static unsigned int id() {
		return ID;
	}

	constexpr static ComponentBitMask mask() {
		return (ComponentBitMask)1 << ID;
	}
};

using CpntPosition = Component<Vector2f, 0>;
using CpntMoving = Component<Vector2f, 1>;

enum class CollsionType
{
	BOX,
	CIRCLE,
	LINE
};

struct CollisionDesc {
	unsigned int group_mask;//自己的mask
	unsigned int collision_mask;//要碰的对象的mask
	CollsionType type;
	union {
		struct {
			float half_width;
			float half_height;
		} box_size;
		struct {
			float r;
		} circle_size;
		struct{
			float width;
		} line_size;
	};

	static CollisionDesc Box(float half_width, float half_height, unsigned int group_mask, unsigned int collision_mask) {
		CollisionDesc cd;
		cd.type = CollsionType::BOX;
		cd.box_size.half_width = half_width;
		cd.box_size.half_height = half_height;
		cd.collision_mask = collision_mask;
		cd.group_mask = group_mask;
		return cd;
	}

};
using CpntCollision = Component<CollisionDesc, 2>;
using CpntHealth = Component<double, 3>;
using CpntCooldown = Component<float, 4>;

enum class RenderType
{
	BOX,
	LINE,
	POINT,
	TEXTURED_BOX
};

struct RenderDesc {
	RenderType type;
	union {
		struct {
			unsigned int width;
			unsigned int height;
			Color color;
		} box;
		struct {
			unsigned int r;
			Color color;
		} circle;
		struct {
			unsigned int width;
			Color color;
		} line;
		struct {
			RES_TEXTURE texture_id;
		} textured_box;
	};

	RenderDesc() {}
	
	static RenderDesc Box(unsigned int width, unsigned int height, Color color) {
		RenderDesc rd;
	
		rd.type = RenderType::BOX;
		rd.box = { .width = width, .height = height, .color = color };
		
		return rd;
	};

	static RenderDesc Textured_box(RES_TEXTURE id) {
		RenderDesc rd;

		rd.type = RenderType::TEXTURED_BOX;
		rd.textured_box.texture_id = id;

		return rd;
	};
};
using CpntRender = Component<RenderDesc, 5>;
using CpntPlayer = Component<void*, 6>;
using CpntDestroying = Component<void*, 7>;

