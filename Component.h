#pragma once

/*
使用ECS架构
1. 系统进行操作，但不记录状态
2. 组件记录状态，就是一坨数据
3. 实体是组件的集合，系统通过操作组件操作实体

4. 

*/
//只保存状态，不存在行为
template <typename T>
class Component{
	T data;
};
