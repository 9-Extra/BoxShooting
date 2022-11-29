#pragma once

#include "Component.h"
#include "World.h"

//代表游戏中的某一类实体，实际上就是一堆Component的集合
//用于方便实体的创建，以及表现实体的继承关系
//由于实体其实完全是system管理的，所以GameObject这些代码只能由system调用，对于游戏对象的实际操作都应该放在system中完成
//GameObject类内部实际不能保存任何数据，所有的操作实际上都必须落到对world的操作
//只不过有了这一层抽象后system里的代码会好看一点
template<ComponentType... ARGS>
class _GameObject abstract {
public:
	consteval static ComponentBitMask basic_components_mask() {
		return EMPTY_MASK;
	}
};

template<ComponentType C, ComponentType... ARGS>
class _GameObject<C, ARGS...> abstract: _GameObject<ARGS...>
{
public:
	//获取该对象的component组（如果用于访问而非创建，这代表应该有的基础components）
	consteval static ComponentBitMask basic_components_mask() {
		return _GameObject<ARGS...>::basic_components_mask() | C::mask();
	}
};

template<ComponentType... ARGS>
class GameObject: public _GameObject<ARGS...>{
public:
	const unsigned int id;
	World& world;

	using _GameObject<ARGS...>::basic_components_mask;

	//直接创建GameObject并非真正在world中创建了一个实体，而是提供了一个访问此id对应实体的代理对象
	GameObject(World& world, unsigned int id)
		:id(id), world(world)
	{}

	//创建实体
	void create() {
		assert(!is_exist());
		actual_components_mask() = basic_components_mask();
	}

	bool is_exist() {
		return actual_components_mask() != EMPTY_MASK;
	}
	
	//这个对象与world中的实际类型相匹配吗
	bool is_match() {
		return mask_contain(actual_components_mask(), basic_components_mask());
	}

	bool is_alive() {
		return !mask_contain(actual_components_mask(), CpntDestroying::mask());
	}

	template<ComponentType C>
	C::DataType& component_data() {
		if constexpr (std::is_same<C, CpntPosition>::value) {
			return world.cpnt_position[id].data;
		}
		else if constexpr (std::is_same<C, CpntMoving>::value) {
			return world.cpnt_moving[id].data;
		}
		else if constexpr (std::is_same<C, CpntCollision>::value) {
			return world.cpnt_collision[id].data;
		}
		else if constexpr (std::is_same<C, CpntHealth>::value) {
			return nullptr;
		}
		else if constexpr (std::is_same<C, CpntCooldown>::value) {
			return world.cpnt_cooldown[id].data;
		}
		else if constexpr (std::is_same<C, CpntRender>::value) {
			return world.cpnt_render[id].data;
		}
		else if constexpr (std::is_same<C,CpntAI>::value) {
			return world.cpnt_ai[id].data;
		}
		else if constexpr (std::is_same<C, CpntDestroying>::value) {
			GameError(L"组件不存在对象");
		}
	}

	ComponentBitMask& actual_components_mask() {
		return world.entites[id].components;
	}
};
